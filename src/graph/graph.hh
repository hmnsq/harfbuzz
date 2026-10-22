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

#جيڪڏهن_وصف_نه_ٿيل گراف_گراف_HH
#وصف_ڪريو گراف_گراف_HH

#وصف_ڪريو HB_گراف_INVALID ((بنانشان) -1)

نطاق گراف {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
بنية گراف_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  بنية راس_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    بنانشان space = 0 ;
    بنانشان شروعات = 0;
    بنانشان پڇاڙي = 0;
    بنانشان ترجيح = 0;

    خاص:
    hb_serialize_context_t::object_t obj_;
    بنانشان incoming_ڪنارا = 0;
    بنانشان single_والد = HB_گراف_INVALID;
    منطقي has_incoming_virtual_ڪنارا = خطأ;
    hb_hashmap_t<بنانشان, بنانشان> parents;
    hb_سيٽ_t virtual_parents;
    عام:

    راس_t ()
    {}

    واضح راس_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    خودڪار parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_والد, single_والد != HB_گراف_INVALID),
	parents.ڪنجيون_ref ()
      )
    )

    فراغ سيٽ_بفر (حرف* سر_2, حرف* پڇ) {
      obj_.سر_2 = سر_2;
      obj_.پڇ = پڇ;
    }

    فراغ shrink_بفر (size_t ڳڻپ)
    {
      obj_.پڇ -= ڳڻپ;
    }

    خودڪار all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    خودڪار real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    خودڪار virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    فراغ ترتيب_ڏيو_real_links () {
      obj_.real_links.qsort ();
    }

    گراف_نتيجو_t<فراغ> جوڙيو_real_link (بنانشان ويڪر, بنانشان obj_idx, بنانشان position) {
      خودڪار* link = obj_.real_links.push ();
      TRY (گراف_نتيجو_t<فراغ>::from (obj_.real_links, ورهاست_FAILURE));
      link->ويڪر = ويڪر;
      link->objidx = obj_idx;
      link->position = position;
      أرجع Ok();
    }

    گراف_نتيجو_t<فراغ> جوڙيو_virtual_link (بنانشان obj_idx) {
      خودڪار* link = obj_.virtual_links.push ();
      TRY (گراف_نتيجو_t<فراغ>::from (obj_.virtual_links, ورهاست_FAILURE));
      link->objidx = obj_idx;
      أرجع Ok();
    }

    گراف_نتيجو_t<فراغ> جوڙيو_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      أرجع گراف_نتيجو_t<فراغ>::from (obj_.real_links, ورهاست_FAILURE);
    }

    گراف_نتيجو_t<فراغ> سيٽ_real_links (hb_ويڪٽر_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(گراف_نتيجو_t<فراغ>::from (links, ورهاست_FAILURE));
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

    گراف_نتيجو_t<فراغ> جوڙيو_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      أرجع گراف_نتيجو_t<فراغ>::from (obj_.virtual_links, ورهاست_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { أرجع obj_; }

    سانچو<قسم_نالو T>
    T as () ثابت
    {
      أرجع reinterpret_cast<T> (obj_.سر_2);
    }

    منطقي has_incoming_virtual_ڪنارا_2 () ثابت
    {
      أرجع has_incoming_virtual_ڪنارا;
    }

    گراف_نتيجو_t<فراغ> link_positions_valid (بنانشان انگ_objects, منطقي removed_nil)
    {
      hb_سيٽ_t assigned_بائيٽون;
      لكل (ثابت خودڪار& l : obj_.real_links)
      {
        إذا (unlikely (l.objidx >= انگ_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                     "Invalid graph. Invalid object index.");
          أرجع Err(INVALID_دليل);
        }

        بنانشان شروعات = l.position;
        بنانشان پڇاڙي = شروعات + l.ويڪر - 1;

        إذا (unlikely (l.ويڪر < 2 || l.ويڪر > 4))
        {
          DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                     "Invalid graph. Invalid link width.");
          أرجع Err(INVALID_دليل);
        }

        إذا (unlikely (پڇاڙي >= جدول_سائيز ()))
        {
          DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                     "Invalid graph. Link position is out of bounds.");
          أرجع Err(ٻاهريون_جو_BOUNDS);
        }

        إذا (unlikely (assigned_بائيٽون.intersects (شروعات, پڇاڙي)))
        {
          DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                     "Invalid graph. Found offsets whose positions overlap.");
          أرجع Err(INVALID_دليل);
        }

        assigned_بائيٽون.جوڙيو_range (شروعات, پڇاڙي);
      }

      أرجع گراف_نتيجو_t<فراغ>::from (assigned_بائيٽون, ورهاست_FAILURE);
    }

    فراغ normalize ()
    {
      obj_.real_links.qsort ();
      لكل (ثابت خودڪار& l : obj_.real_links)
      {
        لكل (بنانشان i = 0; i < l.ويڪر; i++)
        {
          obj_.سر_2[l.position + i] = 0;
        }
      }
    }

    منطقي equals (بنانشان this_اشاريو,
                 بنانشان other_اشاريو,
                 ثابت راس_t& other,
                 ثابت گراف_t& گراف,
                 ثابت گراف_t& other_گراف,
                 بنانشان اونهائي) ثابت
    {
      إذا (!(as_بائيٽون () == other.as_بائيٽون ()))
      {
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_اشاريو,
                   (بنانشان ڊگهو) جدول_سائيز (),
                   other_اشاريو,
                   (بنانشان ڊگهو) other.جدول_سائيز (),
                   اونهائي);

        خودڪار a = as_بائيٽون ();
        خودڪار b = other.as_بائيٽون ();
        طالما (a || b)
        {
          DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                     "  0x%x %s 0x%x", (بنانشان) *a, (*a == *b) ? "==" : "!=", (بنانشان) *b);
          a++;
          b++;
        }
        أرجع خطأ;
      }

      أرجع links_equal (obj_.real_links, other.obj_.real_links, گراف, other_گراف, اونهائي);
    }

    hb_بائيٽون_t as_بائيٽون () ثابت
    {
      أرجع hb_بائيٽون_t (obj_.سر_2, جدول_سائيز ());
    }

    دوست فراغ swap (راس_t& a, راس_t& b)
    {
      hb_مٽا_سٽا (a.obj_, b.obj_);
      hb_مٽا_سٽا (a.distance, b.distance);
      hb_مٽا_سٽا (a.space, b.space);
      hb_مٽا_سٽا (a.single_والد, b.single_والد);
      hb_مٽا_سٽا (a.parents, b.parents);
      hb_مٽا_سٽا (a.virtual_parents, b.virtual_parents);
      hb_مٽا_سٽا (a.incoming_ڪنارا, b.incoming_ڪنارا);
      hb_مٽا_سٽا (a.has_incoming_virtual_ڪنارا, b.has_incoming_virtual_ڪنارا);
      hb_مٽا_سٽا (a.شروعات, b.شروعات);
      hb_مٽا_سٽا (a.پڇاڙي, b.پڇاڙي);
      hb_مٽا_سٽا (a.ترجيح, b.ترجيح);
    }

    گراف_نتيجو_t<hb_hashmap_t<بنانشان, بنانشان>>
    position_منزل_اشاريو_map () ثابت
    {
      hb_hashmap_t<بنانشان, بنانشان> نتيجو;

      نتيجو.alloc (obj_.real_links.ڊيگهائي);
      لكل (ثابت خودڪار& l : obj_.real_links) {
        نتيجو.سيٽ (l.position, l.objidx);
      }

      أرجع گراف_نتيجو_t<hb_hashmap_t<بنانشان, بنانشان>>::from (std::move(نتيجو), ورهاست_FAILURE);
    }

    منطقي is_shared () ثابت
    {
      أرجع parents.get_population () > virtual_parents.get_population () + 1;
    }

    بنانشان incoming_ڪنارا_2 () ثابت
    {
      إذا (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_ڪنارا == (single_والد != HB_گراف_INVALID) +
		(parents.قدرون_ref () | hb_reduce (hb_جوڙيو, 0)));
       }
      أرجع incoming_ڪنارا;
    }

    بنانشان incoming_ڪنارا_from_والد (بنانشان والد_اشاريو) ثابت {
      إذا (single_والد != HB_گراف_INVALID) {
        أرجع single_والد == والد_اشاريو ? 1 : 0;
      }

      بنانشان* ڳڻپ;
      أرجع  parents.has(والد_اشاريو, &ڳڻپ) ? *ڳڻپ : 0;
    }

    فراغ reset_parents ()
    {
      incoming_ڪنارا = 0;
      has_incoming_virtual_ڪنارا = خطأ;
      single_والد = HB_گراف_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    گراف_نتيجو_t<فراغ> جوڙيو_والد (بنانشان والد_اشاريو, منطقي is_virtual)
    {
      أكد (والد_اشاريو != HB_گراف_INVALID);
      has_incoming_virtual_ڪنارا |= is_virtual;
      إذا (is_virtual)
        virtual_parents.جوڙيو (والد_اشاريو);

      إذا (incoming_ڪنارا == 0)
      {
	single_والد = والد_اشاريو;
	incoming_ڪنارا = 1;
	أرجع منزل_نتيجو();
      }
      وإلا إذا (single_والد != HB_گراف_INVALID)
      {
        أكد (incoming_ڪنارا == 1);
	إذا (!parents.سيٽ (single_والد, 1))
	  أرجع منزل_نتيجو ();
	single_والد = HB_گراف_INVALID;
      }

      بنانشان *v;
      إذا (parents.has (والد_اشاريو, &v))
      {
        (*v)++;
	incoming_ڪنارا++;
      }
      وإلا إذا (parents.سيٽ (والد_اشاريو, 1))
	incoming_ڪنارا++;

      أرجع منزل_نتيجو();
    }

    فراغ remove_والد (بنانشان والد_اشاريو)
    {
      إذا (والد_اشاريو == single_والد)
      {
	single_والد = HB_گراف_INVALID;
	incoming_ڪنارا--;
	virtual_parents.reset ();
	أرجع;
      }

      بنانشان *v;
      إذا (parents.has (والد_اشاريو, &v))
      {
	incoming_ڪنارا--;
	إذا (*v > 1)
	  (*v)--;
	وإلا
        {
	  parents.del (والد_اشاريو);
          virtual_parents.del (والد_اشاريو);
        }

	إذا (incoming_ڪنارا == 1)
	{
	  single_والد = *parents.ڪنجيون ();
	  parents.reset ();
	}
      }
    }

    فراغ remove_real_link_unordered (بنانشان link_array_اشاريو) {
      obj_.real_links.remove_unordered (link_array_اشاريو);
    }

    فراغ remove_real_link_unordered (بنانشان ٻار_اشاريو, ثابت فراغ* وٿي)
    {
      بنانشان ڳڻپ = obj_.real_links.ڊيگهائي;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
        خودڪار& link = obj_.real_links.arrayZ[i];
        إذا (link.objidx != ٻار_اشاريو)
          استمر;

        إذا ((obj_.سر_2 + link.position) != وٿي)
          استمر;

        obj_.real_links.remove_unordered (i);
        أرجع;
      }
    }

    گراف_نتيجو_t<فراغ> remap_والد (بنانشان old_اشاريو, بنانشان new_اشاريو)
    {
      إذا (single_والد != HB_گراف_INVALID)
      {
        إذا (single_والد == old_اشاريو)
        {
	  single_والد = new_اشاريو;
          إذا (virtual_parents.has (old_اشاريو))
          {
            virtual_parents.del (old_اشاريو);
            virtual_parents.جوڙيو (new_اشاريو);
          }
        }
        أرجع گراف_نتيجو_t<فراغ>::from(virtual_parents, ورهاست_FAILURE);
      }

      ثابت بنانشان *pv;
      إذا (parents.has (old_اشاريو, &pv))
      {
        بنانشان v = *pv;
	إذا (!parents.سيٽ (new_اشاريو, v))
          incoming_ڪنارا -= v;
	parents.del (old_اشاريو);

        إذا (incoming_ڪنارا == 1)
	{
	  single_والد = *parents.ڪنجيون ();
	  parents.reset ();
	}
      }

      إذا (virtual_parents.has (old_اشاريو))
      {
        virtual_parents.del (old_اشاريو);
        virtual_parents.جوڙيو (new_اشاريو);
      }

      أرجع گراف_نتيجو_t<فراغ>::from(virtual_parents, ورهاست_FAILURE);
    }

    منطقي is_پن () ثابت
    {
      أرجع !obj_.real_links.ڊيگهائي && !obj_.virtual_links.ڊيگهائي;
    }

    منطقي raise_ترجيح ()
    {
      إذا (has_max_ترجيح ()) أرجع خطأ;
      ترجيح++;
      أرجع سچ;
    }

    منطقي give_max_ترجيح ()
    {
      منطقي نتيجو = خطأ;
      طالما (!has_max_ترجيح()) {
        نتيجو = سچ;
        ترجيح++;
      }
      أرجع نتيجو;
    }

    منطقي has_max_ترجيح () ثابت {
      أرجع ترجيح >= 3;
    }

    size_t جدول_سائيز () ثابت {
      أرجع obj_.پڇ - obj_.سر_2;
    }

    int64_t modified_مفاصلو (بنانشان ترتيب) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_مفاصلو =
          hb_clamp (distance + مفاصلو_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      إذا (has_max_ترجيح ()) {
        modified_مفاصلو = 0;
      }
      أرجع (modified_مفاصلو << 18) | (0x003FFFF & ترتيب);
    }

    int64_t مفاصلو_modifier () ثابت
    {
      إذا (!ترجيح) أرجع 0;
      int64_t جدول_سائيز = obj_.پڇ - obj_.سر_2;

      إذا (ترجيح == 1)
        أرجع -جدول_سائيز / 2;

      أرجع -جدول_سائيز;
    }

   خاص:
    گراف_نتيجو_t<فراغ> منزل_نتيجو() ثابت {
      إذا (unlikely (
        parents.اندريون_error() || virtual_parents.اندريون_error() ||
        obj_.real_links.اندريون_error() || obj_.virtual_links.اندريون_error()))
        أرجع Err(ورهاست_FAILURE);
      أرجع Ok();
    }

    منطقي links_equal (ثابت hb_ويڪٽر_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_ويڪٽر_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت گراف_t& گراف,
                      ثابت گراف_t& other_گراف,
                      بنانشان اونهائي) ثابت
    {
      خودڪار a = this_links.iter ();
      خودڪار b = other_links.iter ();

      طالما (a && b)
      {
        ثابت خودڪار& link_a = *a;
        ثابت خودڪار& link_b = *b;

        إذا (link_a.ويڪر != link_b.ويڪر ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          أرجع خطأ;

        إذا (!گراف.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_گراف.vertices_[link_b.objidx], گراف, other_گراف, اونهائي + 1))
          أرجع خطأ;

        a++;
        b++;
      }

      إذا (منطقي (a) != منطقي (b))
        أرجع خطأ;

      أرجع سچ;
    }
  };

  تعداد راس_mutability_t
  {
    Immutable,
    Mutable
  };

  سانچو <قسم_نالو T, راس_mutability_t mutability = Mutable>
  بنية راس_and_جدول_t
  {
    راس_and_جدول_t () : اشاريو_2 (0), راس (خالي_اشارو), جدول (خالي_اشارو)
    {}

    سانچو <راس_mutability_t other_mutability>
    راس_and_جدول_t (ثابت راس_and_جدول_t<T, other_mutability>& o)
      : اشاريو_2 (o.اشاريو_2), راس (o.راس), جدول (o.جدول)
    {}

    بنانشان اشاريو_2;
    قسم_نالو std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type راس;
    قسم_نالو std::conditional<mutability == Immutable, ثابت T*, T*>::type جدول;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  سانچو<قسم_نالو T>
  ساكن گراف_نتيجو_t<گراف_t> create (ثابت T& objects)
  {
    إذا (unlikely (objects.ڊيگهائي > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.ڊيگهائي);
      أرجع Err(حد_بندي_EXCEEDED);
    }

    گراف_t g;
    g.انگ_roots_for_space.push (1);
    TRY (گراف_نتيجو_t<فراغ>::from (g.انگ_roots_for_space, ورهاست_FAILURE));

    منطقي removed_nil = خطأ;
    ثابت بنانشان ڳڻپ = objects.ڊيگهائي;
    g.vertices_.alloc (ڳڻپ);
    g.ordering_.resize (ڳڻپ);
    g.ordering_scratch_.alloc (ڳڻپ);
    TRY (گراف_نتيجو_t<فراغ>::from (g.vertices_, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (g.ordering_, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (g.ordering_scratch_, ورهاست_FAILURE));

    بنانشان ترتيب = objects.ڊيگهائي;
    بنانشان skip = 0;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      إذا (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = سچ;
        ترتيب--;
        g.ordering_.resize (ڳڻپ - 1);
        skip++;
        استمر;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      إذا (unlikely (obj.اندريون_error ())) {
        أرجع Err(ورهاست_FAILURE);
      }
      راس_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (ڳڻپ, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      بنانشان obj_idx = i - skip;
      g.ordering_[--ترتيب] = obj_idx;

      إذا (!removed_nil) استمر;
      // Fix indices to account for removed nil object.
      لكل (خودڪار& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    خودڪار r = g.is_fully_connected();
    إذا (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    أرجع Ok (std::move (g));
  }

  گراف_t (ثابت گراف_t&) = ميٽيو;
  گراف_t& عامل = (ثابت گراف_t&) = ميٽيو;
  گراف_t (گراف_t&&) = افتراضي;
  گراف_t& عامل = (گراف_t&&) = افتراضي;

  ~گراف_t ()
  {
    لكل (حرف* b : buffers)
      hb_free (b);
  }

خاص:
  گراف_t () = افتراضي;

عام:

  منطقي عامل== (ثابت گراف_t& other) ثابت
  {
    أرجع پاڙ ().equals (پاڙ_idx(), other.پاڙ_idx(), other.پاڙ (), *هذا, other, 0);
  }

  فراغ print () ثابت {
    لكل (بنانشان id : ordering_)
    {
      ثابت خودڪار& v = vertices_[id];
      لکو("%u: %u [", id, (بنانشان صحيح)v.جدول_سائيز());
      لكل (ثابت خودڪار &l : v.obj().real_links) {
        لکو("%u, ", l.objidx);
      }
      لكل (ثابت خودڪار &l : v.obj().virtual_links) {
        لکو("v%u, ", l.objidx);
      }
      لکو("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  فراغ normalize ()
  {
    لكل (خودڪار& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت راس_t& پاڙ () ثابت
  {
    أرجع vertices_[پاڙ_idx ()];
  }

  بنانشان پاڙ_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    أرجع ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (بنانشان i) ثابت
  {
    أرجع vertices_[i].obj();
  }

  گراف_نتيجو_t<فراغ> جوڙيو_بفر (حرف* بفر)
  {
    buffers.push (بفر);
    أرجع گراف_نتيجو_t<فراغ>::from (buffers, ورهاست_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  سانچو<قسم_نالو T>
  گراف_نتيجو_t<فراغ> جوڙيو_link (T* وٿي,
                                 بنانشان والد_id,
                                 بنانشان ٻار_id)
  {
    إذا (unlikely (والد_id >= vertices_.ڊيگهائي || ٻار_id >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);
    خودڪار& v = vertices_[والد_id];
    TRY(v.جوڙيو_real_link(2, ٻار_id, (حرف*) وٿي - (حرف*) v.obj().سر_2));
    أرجع vertices_[ٻار_id].جوڙيو_والد (والد_id, خطأ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  گراف_نتيجو_t<فراغ> ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو_if_needed ()
  {
    إذا (!positions_invalid) أرجع Ok();
    أرجع ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  گراف_نتيجو_t<فراغ> ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ()
  {
    positions_invalid = سچ;

    إذا (vertices_.ڊيگهائي <= 1) {
      // Graph of 1 or less doesn't need sorting.
      أرجع Ok();
    }

    TRY (update_distances ());

    hb_ترجيح_قطار_t<int64_t> قطار;
    قطار.alloc (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (قطار, ورهاست_FAILURE));

    hb_ويڪٽر_t<بنانشان> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (new_ordering, ورهاست_FAILURE));

    hb_ويڪٽر_t<بنانشان> removed_ڪنارا;
    removed_ڪنارا.resize (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (removed_ڪنارا, ورهاست_FAILURE));

    TRY (update_parents ());

    قطار.insert (پاڙ ().modified_مفاصلو (0), پاڙ_idx ());
    بنانشان ترتيب = 1;
    بنانشان pos = 0;
    طالما (!قطار.اندريون_error () && !قطار.is_empty ())
    {
      بنانشان ايندڙ_id = قطار.ڪڍو_minimum().second;

      إذا (unlikely (pos >= new_ordering.ڊيگهائي)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Invalid graph. Contains cycle.");
        أرجع Err(چڪر_DETECTED);
      }
      new_ordering[pos++] = ايندڙ_id;
      ثابت راس_t& next = vertices_[ايندڙ_id];

      لكل (ثابت خودڪار& link : next.obj().all_links ()) {
        إذا (unlikely (link.objidx >= vertices_.ڊيگهائي))
          أرجع Err(ٻاهريون_جو_BOUNDS);
        removed_ڪنارا[link.objidx]++;
        ثابت خودڪار& v = vertices_[link.objidx];
        إذا (!(v.incoming_ڪنارا_2 () - removed_ڪنارا[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          قطار.insert (v.modified_مفاصلو (ترتيب++),
                        link.objidx);
      }
    }

    TRY (گراف_نتيجو_t<فراغ>::from (قطار, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (new_ordering, ورهاست_FAILURE));

    hb_مٽا_سٽا (ordering_, new_ordering);

    إذا (unlikely (pos != vertices_.ڊيگهائي)) {
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
  گراف_نتيجو_t<فراغ> ڳوليو_space_roots (hb_سيٽ_t& گهميل, hb_سيٽ_t& roots)
  {
    بنانشان پاڙ_اشاريو = پاڙ_idx ();
    لكل (بنانشان i : ordering_)
    {
      إذا (گهميل.has (i)) استمر;

      // Only real links can form 32 bit spaces
      لكل (ثابت خودڪار& l : vertices_[i].obj().real_links)
      {
        إذا (l.is_signed || l.ويڪر < 3)
          استمر;

        إذا (i == پاڙ_اشاريو && l.ويڪر == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          استمر;

        إذا (l.ويڪر >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_سيٽ_t sub_roots;
          TRY (ڳوليو_32bit_roots (l.objidx, sub_roots));
          إذا (sub_roots) {
            لكل (بنانشان sub_پاڙ_idx : sub_roots) {
              roots.جوڙيو (sub_پاڙ_idx);
              TRY (ڳوليو_subgraph (sub_پاڙ_idx, گهميل));
            }
            استمر;
          }
        }

        roots.جوڙيو (l.objidx);
        TRY (ڳوليو_subgraph (l.objidx, گهميل));
      }
    }

    TRY(گراف_نتيجو_t<فراغ>::from (گهميل, ورهاست_FAILURE));
    TRY(گراف_نتيجو_t<فراغ>::from (roots, ورهاست_FAILURE));
    أرجع Ok();
  }

  سانچو <قسم_نالو T, قسم_نالو ...Ts>
  گراف_نتيجو_t<راس_and_جدول_t<T, Immutable>> as_جدول (بنانشان والد, ثابت فراغ* وٿي, Ts... ds)
  {
    TRY_سونپيو (بنانشان idx, اشاريو_for_وٿي (والد, وٿي));
    أرجع as_جدول_from_اشاريو<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچو <قسم_نالو T, قسم_نالو ...Ts>
  گراف_نتيجو_t<راس_and_جدول_t<T, Mutable>> as_mutable_جدول (بنانشان والد, ثابت فراغ* وٿي, Ts... ds)
  {
    TRY_سونپيو (خودڪار idx, mutable_اشاريو_for_وٿي (والد, وٿي));
    أرجع as_جدول_from_اشاريو<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچو <قسم_نالو T, راس_mutability_t mutability = Mutable, قسم_نالو ...Ts>
  گراف_نتيجو_t<راس_and_جدول_t<T, mutability>> as_جدول_from_اشاريو (بنانشان اشاريو_2, Ts... ds)
  {
    إذا (unlikely (اشاريو_2 >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    راس_and_جدول_t<T, mutability> r;
    r.راس = (قسم_نالو std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type) &vertices_[اشاريو_2];
    r.جدول = r.راس->سانچو as<قسم_نالو std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.اشاريو_2 = اشاريو_2;
    إذا (unlikely (!r.جدول))
      أرجع Err(INVALID_دليل);

    TRY(r.جدول->sanitize (*(r.راس), std::forward<Ts>(ds)...));
    أرجع r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  گراف_نتيجو_t<بنانشان> اشاريو_for_وٿي (بنانشان ڳنڍ_idx, ثابت فراغ* وٿي) ثابت
  {
    إذا (unlikely (ڳنڍ_idx >= vertices_.ڊيگهائي)) أرجع Err(ٻاهريون_جو_BOUNDS);
    ثابت خودڪار& node = object (ڳنڍ_idx);
    إذا (unlikely (وٿي < node.سر_2 || وٿي >= node.پڇ)) أرجع Err(ٻاهريون_جو_BOUNDS);

    بنانشان ڳڻپ = node.real_links.ڊيگهائي;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت خودڪار& link = node.real_links.arrayZ[i];
      إذا (وٿي != node.سر_2 + link.position)
        استمر;
      أرجع Ok(link.objidx);
    }

    أرجع Err(INVALID_دليل);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  گراف_نتيجو_t<بنانشان> mutable_اشاريو_for_وٿي (بنانشان ڳنڍ_idx, ثابت فراغ* وٿي)
  {
    TRY_سونپيو (بنانشان ٻار_idx, اشاريو_for_وٿي (ڳنڍ_idx, وٿي));
    إذا (unlikely (ٻار_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    خودڪار& ٻار = vertices_[ٻار_idx];
    لكل (بنانشان p : ٻار.parents_iter ())
    {
      إذا (p != ڳنڍ_idx) {
        أرجع duplicate (ڳنڍ_idx, ٻار_idx, سچ);
      }
    }

    أرجع Ok(ٻار_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  گراف_نتيجو_t<منطقي> سونپيو_spaces ()
  {
    TRY (update_parents ());

    لكل (خودڪار& راس : vertices_.writer ())
    {
      راس.space = 0;
      راس.ترجيح = 0;
    }
    انگ_roots_for_space.resize (1);
    TRY (گراف_نتيجو_t<فراغ>::from (انگ_roots_for_space, ورهاست_FAILURE));
    انگ_roots_for_space[0] = 1;
    مفاصلو_invalid = سچ;
    positions_invalid = سچ;

    hb_سيٽ_t گهميل;
    hb_سيٽ_t roots;
    TRY (ڳوليو_space_roots (گهميل, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    گهميل.invert ();

    إذا (!roots) أرجع Ok(خطأ);

    طالما (roots)
    {
      uint32_t next = HB_سيٽ_قدر_INVALID;
      TRY (گراف_نتيجو_t<فراغ>::from (roots, ورهاست_FAILURE));
      إذا (!roots.next (&next)) اكسر;

      hb_سيٽ_t connected_roots;
      TRY (ڳوليو_connected_nodes (next, roots, گهميل, connected_roots));
      TRY (گراف_نتيجو_t<فراغ>::from (connected_roots, ورهاست_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (گراف_نتيجو_t<فراغ>::from (connected_roots, ورهاست_FAILURE));

      بنانشان ايندڙ_space = هذا->ايندڙ_space ();
      إذا (unlikely (ايندڙ_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 ايندڙ_space);
        أرجع Err(حد_بندي_EXCEEDED);
      }
      انگ_roots_for_space.push (0);
      TRY(گراف_نتيجو_t<فراغ>::from (انگ_roots_for_space, ورهاست_FAILURE));

      لكل (بنانشان پاڙ : connected_roots)
      {
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Subgraph %u gets space %u", پاڙ, ايندڙ_space);
        vertices_[پاڙ].space = ايندڙ_space;
        انگ_roots_for_space[ايندڙ_space] = انگ_roots_for_space[ايندڙ_space] + 1;
        مفاصلو_invalid = سچ;
        positions_invalid = سچ;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    أرجع Ok(سچ);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  گراف_نتيجو_t<منطقي> isolate_subgraph (hb_سيٽ_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_سيٽ_t parents;
    لكل (بنانشان پاڙ_idx : roots)
    {
      إذا (unlikely (پاڙ_idx >= vertices_.ڊيگهائي))
        أرجع Err(ٻاهريون_جو_BOUNDS);
      subgraph.سيٽ (پاڙ_idx, wide_parents (پاڙ_idx, parents));
    }
    لكل (بنانشان پاڙ_idx : roots)
    {
      TRY (ڳوليو_subgraph (پاڙ_idx, subgraph));
    }
    TRY (گراف_نتيجو_t<فراغ>::from (subgraph, ورهاست_FAILURE));

    hb_map_t اشاريو_map;
    منطقي made_changes = خطأ;
    لكل (خودڪار داخلا : subgraph.iter ())
    {
      أكد (داخلا.پهريون < vertices_.ڊيگهائي);
      ثابت خودڪار& node = vertices_[داخلا.پهريون];
      بنانشان subgraph_incoming_ڪنارا = داخلا.second;

      إذا (subgraph_incoming_ڪنارا < node.incoming_ڪنارا_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = سچ;
        TRY (duplicate_subgraph (داخلا.پهريون, اشاريو_map));
      }
    }

    TRY (گراف_نتيجو_t<فراغ>::from (اشاريو_map, ورهاست_FAILURE));

    إذا (!made_changes)
      أرجع Ok(خطأ);

    خودڪار new_subgraph =
        + subgraph.ڪنجيون ()
        | hb_map([&] (uint32_t ڳنڍ_idx) {
          ثابت uint32_t *v;
          إذا (اشاريو_map.has (ڳنڍ_idx, &v)) أرجع *v;
          أرجع ڳنڍ_idx;
        })
        ;

    TRY(remap_obj_indices (اشاريو_map, new_subgraph));
    TRY(remap_obj_indices (اشاريو_map, parents.iter (), سچ));

    // Update roots set with new indices as needed.
    لكل (خودڪار next : roots)
    {
      ثابت uint32_t *v;
      إذا (اشاريو_map.has (next, &v))
      {
        roots.del (next);
        roots.جوڙيو (*v);
      }
    }

    أرجع Ok(سچ);
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
  سانچو <قسم_نالو گهمو_ڪنارو_Func>
  گراف_نتيجو_t<فراغ> traverse_directed_bfs (بنانشان شروعات_idx, گهمو_ڪنارو_Func&& گهمو_ڪنارو)
  {
    إذا (unlikely (شروعات_idx >= vertices_.ڊيگهائي))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      أرجع Err(ٻاهريون_جو_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (ordering_scratch_, ورهاست_FAILURE));

    إذا (!گهمو_ڪنارو (HB_CODEPOINT_INVALID, خالي_اشارو, شروعات_idx, 0))
      أرجع Ok();

    بنانشان سر_2 = 0;
    بنانشان پڇ = 0;
    خودڪار& قطار = ordering_scratch_;
    قطار[پڇ++] = شروعات_idx;

    بنانشان اونهائي = 0;
    طالما (سر_2 < پڇ)
    {
      بنانشان level_پڇاڙي = پڇ;
      طالما (سر_2 < level_پڇاڙي)
      {
        بنانشان ڳنڍ_idx = قطار[سر_2++];
        ثابت خودڪار& v = vertices_[ڳنڍ_idx];

        بنانشان انگ_real = v.obj().real_links.ڊيگهائي;
        بنانشان ڪل_links = انگ_real + v.obj().virtual_links.ڊيگهائي;

        لكل (بنانشان i = 0; i < ڪل_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت خودڪار& v = vertices_[ڳنڍ_idx];
          ثابت خودڪار& link = (i < انگ_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - انگ_real];
          بنانشان ٻار_idx = link.objidx;

          إذا (!گهمو_ڪنارو (ڳنڍ_idx, &link, ٻار_idx, اونهائي + 1))
            استمر;

          إذا (unlikely (پڇ >= قطار.ڊيگهائي))
            أرجع Err(چڪر_DETECTED);

          قطار[پڇ++] = ٻار_idx;
        }
      }
      اونهائي++;
    }
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> ڳوليو_subgraph (بنانشان ڳنڍ_idx, hb_map_t& subgraph)
  {
    أرجع traverse_directed_bfs (ڳنڍ_idx, [&] (
      بنانشان والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشان ٻار,
      بنانشان اونهائي) {
      إذا (اونهائي == 0) أرجع سچ;
      hb_codepoint_t *ڳڻپ;
      إذا (subgraph.has (ٻار, &ڳڻپ))
      {
        (*ڳڻپ)++;
        أرجع خطأ;
      }
      subgraph.سيٽ (ٻار, 1);
      أرجع سچ;
    });
  }

  گراف_نتيجو_t<فراغ> ڳوليو_subgraph (بنانشان ڳنڍ_idx, hb_سيٽ_t& subgraph)
  {
    أرجع traverse_directed_bfs (ڳنڍ_idx, [&] (
      بنانشان والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشان ٻار,
      بنانشان اونهائي) {
      إذا (subgraph.has (ٻار)) أرجع خطأ;
      subgraph.جوڙيو (ٻار);
      أرجع سچ;
    });
  }

  گراف_نتيجو_t<size_t> ڳوليو_subgraph_سائيز (بنانشان ڳنڍ_idx, hb_سيٽ_t& subgraph, بنانشان max_اونهائي = HB_گراف_INVALID)
  {
    size_t سائيز = 0;
    TRY (traverse_directed_bfs (ڳنڍ_idx, [&] (
      بنانشان والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشان ٻار,
      بنانشان اونهائي) {
      إذا (subgraph.has (ٻار)) أرجع خطأ;
      subgraph.جوڙيو (ٻار);

      ثابت خودڪار& o = vertices_[ٻار].obj();
      سائيز += o.پڇ - o.سر_2;
      أرجع اونهائي < max_اونهائي;
    }));
    أرجع Ok(سائيز);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  گراف_نتيجو_t<فراغ> ڳوليو_32bit_roots (بنانشان ڳنڍ_idx, hb_سيٽ_t& مليو)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_سيٽ_t گهميل;
    أرجع traverse_directed_bfs (ڳنڍ_idx, [&] (
      بنانشان والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشان ٻار,
      بنانشان _) {

      إذا (link && مليو.has(والد))
        // Don't traverse from something that's already marked as a root.
        أرجع خطأ;

      إذا (link && !link->is_signed && link->ويڪر == 4)
      {
        مليو.جوڙيو (link->objidx);
        گهميل.جوڙيو (link->objidx);
        أرجع خطأ;
      }

      إذا (گهميل.has (ٻار)) أرجع خطأ;
      گهميل.جوڙيو (ٻار);
      أرجع سچ;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  سانچو<قسم_نالو O>
  گراف_نتيجو_t<بنانشان> move_ٻار (بنانشان old_والد_idx,
                                       ثابت O* old_وٿي,
                                       بنانشان new_والد_idx,
                                       ثابت O* new_وٿي)
  {
    مفاصلو_invalid = سچ;
    positions_invalid = سچ;

    إذا (unlikely (old_والد_idx >= vertices_.ڊيگهائي ||
                  new_والد_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    خودڪار& old_v = vertices_[old_والد_idx];
    خودڪار& new_v = vertices_[new_والد_idx];

    TRY_سونپيو (بنانشان ٻار_id, اشاريو_for_وٿي (old_والد_idx,
                                                     old_وٿي));
    إذا (unlikely (ٻار_id >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    TRY(new_v.جوڙيو_real_link(O::static_سائيز, ٻار_id, (ثابت حرف*) new_وٿي - (ثابت حرف*) new_v.obj().سر_2));

    خودڪار& ٻار = vertices_[ٻار_id];
    TRY(ٻار.جوڙيو_والد (new_والد_idx, خطأ));

    old_v.remove_real_link_unordered (ٻار_id, old_وٿي);
    ٻار.remove_والد (old_والد_idx);

    أرجع Ok(ٻار_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  سانچو<قسم_نالو O>
  گراف_نتيجو_t<فراغ> move_children (بنانشان old_والد_idx,
                                      بنانشان old_pos_شروعات,
                                      بنانشان old_pos_پڇاڙي,
                                      بنانشان new_والد_idx,
                                      بنانشان new_pos_شروعات)
  {
    إذا (unlikely (old_والد_idx >= vertices_.ڊيگهائي ||
                  new_والد_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    مفاصلو_invalid = سچ;
    positions_invalid = سچ;

    خودڪار& old_v = vertices_[old_والد_idx];
    خودڪار& new_v = vertices_[new_والد_idx];

    hb_ويڪٽر_t<hb_serialize_context_t::object_t::link_t> old_links;
    لكل (ثابت خودڪار& l : old_v.obj().real_links)
    {
      إذا (l.position < old_pos_شروعات || l.position >= old_pos_پڇاڙي)
      {
        old_links.push (l);
        استمر;
      }

      بنانشان array_pos = l.position - old_pos_شروعات;

      بنانشان ٻار_id = l.objidx;
      إذا (unlikely (ٻار_id >= vertices_.ڊيگهائي))
        أرجع Err(ٻاهريون_جو_BOUNDS);

      TRY(new_v.جوڙيو_real_link(O::static_سائيز, ٻار_id, new_pos_شروعات + array_pos));

      خودڪار& ٻار = vertices_[ٻار_id];
      TRY(ٻار.جوڙيو_والد (new_والد_idx, خطأ));
      ٻار.remove_والد (old_والد_idx);
    }

    TRY (گراف_نتيجو_t<فراغ>::from (old_links, ورهاست_FAILURE));
    أرجع old_v.سيٽ_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  گراف_نتيجو_t<فراغ> duplicate_subgraph (بنانشان ڳنڍ_idx, hb_map_t& اشاريو_map)
  {
    گراف_نتيجو_t<فراغ> نتيجو = Ok();
    TRY (traverse_directed_bfs (ڳنڍ_idx, [&] (
      بنانشان والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشان ٻار,
      بنانشان _) {
      إذا (نتيجو.is_err () || اشاريو_map.has (ٻار)) أرجع خطأ;
      خودڪار r = duplicate (ٻار);
      إذا (unlikely (r.is_err ()))
      {
        نتيجو = Err(r.error ());
        أرجع خطأ;
      }
      اشاريو_map.سيٽ (ٻار, *r);
      أرجع سچ;
    }));

    أرجع گراف_نتيجو_t<فراغ>::from(اشاريو_map, ورهاست_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  گراف_نتيجو_t<بنانشان> duplicate (بنانشان ڳنڍ_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (ڳنڍ_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    إذا (unlikely (vertices_.ڊيگهائي >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.ڊيگهائي);
      أرجع Err(حد_بندي_EXCEEDED);
    }

    positions_invalid = سچ;
    مفاصلو_invalid = سچ;

    خودڪار* clone = vertices_.push ();
    TRY (گراف_نتيجو_t<فراغ>::from (vertices_, ورهاست_FAILURE));

    بنانشان clone_idx = vertices_.ڊيگهائي - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتيجو_t<فراغ>::from (ordering_, ورهاست_FAILURE));

    خودڪار& ٻار = vertices_[ڳنڍ_idx];

    بنانشان جدول_سائيز = ٻار.obj().پڇ - ٻار.obj().سر_2;
    إذا (copy_جدول && جدول_سائيز)
    {
      حرف* بفر = (حرف*) hb_malloc (جدول_سائيز);
      إذا (unlikely (!بفر))
        أرجع Err(ورهاست_FAILURE);
      خودڪار res = جوڙيو_بفر (بفر);
      إذا (unlikely (res.is_err ()))
      {
        hb_free (بفر);
        أرجع Err(res.error ());
      }
      hb_memcpy (بفر, ٻار.obj().سر_2, جدول_سائيز);
      clone->سيٽ_بفر(بفر, بفر + جدول_سائيز);
    }
    وإلا
    {
      clone->سيٽ_بفر(ٻار.obj().سر_2, ٻار.obj().پڇ);
    }
    clone->distance = ٻار.distance;
    clone->space = ٻار.space;
    clone->reset_parents ();

    لكل (ثابت خودڪار& l : ٻار.obj().real_links)
    {
      TRY(clone->جوڙيو_real_link (l));
      TRY(vertices_[l.objidx].جوڙيو_والد (clone_idx, خطأ));
    }

    لكل (ثابت خودڪار& l : ٻار.obj().virtual_links)
    {
      TRY(clone->جوڙيو_virtual_link (l));
      TRY(vertices_[l.objidx].جوڙيو_والد (clone_idx, سچ));
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
  گراف_نتيجو_t<بنانشان> duplicate (بنانشان والد_idx, بنانشان ٻار_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (والد_idx >= vertices_.ڊيگهائي || ٻار_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    TRY (update_parents ());

    ثابت خودڪار& ٻار = vertices_[ٻار_idx];
    بنانشان links_منزل_ٻار = ٻار.incoming_ڪنارا_from_والد (والد_idx);

    إذا (unlikely (ٻار.incoming_ڪنارا_2 () <= links_منزل_ٻار || ٻار.has_incoming_virtual_ڪنارا_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "  Not duplicating %u => %u",
                 والد_idx, ٻار_idx);
      أرجع Err(INVALID_دليل);
    }

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "  Duplicating %u => %u",
               والد_idx, ٻار_idx);

    TRY_سونپيو (بنانشان clone_idx, duplicate (ٻار_idx, copy_جدول));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    إذا (والد_idx == clone_idx) والد_idx++;

    خودڪار& والد = vertices_[والد_idx];
    بنانشان ڳڻپ = 0;
    بنانشان انگ_real = والد.obj().real_links.ڊيگهائي;
    لكل (خودڪار& l : والد.all_links_writer ())
    {
      ڳڻپ++;
      إذا (l.objidx != ٻار_idx)
        استمر;

      TRY(reassign_link (l, والد_idx, clone_idx, ڳڻپ > انگ_real));
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
  گراف_نتيجو_t<بنانشان> duplicate (ثابت hb_سيٽ_t* parents, بنانشان ٻار_idx)
  {
    إذا (unlikely (!parents || parents->is_empty ()))
      أرجع Err(INVALID_دليل);

    إذا (unlikely (ٻار_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    TRY (update_parents ());

    ثابت خودڪار& ٻار = vertices_[ٻار_idx];
    بنانشان links_منزل_ٻار = 0;
    بنانشان آخري_والد = parents->get_max ();
    بنانشان پهريون_والد = parents->get_min ();
    لكل (بنانشان والد_idx : *parents) {
      إذا (unlikely (والد_idx >= vertices_.ڊيگهائي))
        أرجع Err(ٻاهريون_جو_BOUNDS);
      links_منزل_ٻار += ٻار.incoming_ڪنارا_from_والد (والد_idx);
    }

    إذا (unlikely (ٻار.incoming_ڪنارا_2 () <= links_منزل_ٻار || ٻار.has_incoming_virtual_ڪنارا_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "  Not duplicating %u, ..., %u => %u", پهريون_والد, آخري_والد, ٻار_idx);
      أرجع Err(INVALID_دليل);
    }

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "  Duplicating %u, ..., %u => %u", پهريون_والد, آخري_والد, ٻار_idx);

    TRY_سونپيو (بنانشان clone_idx, duplicate (ٻار_idx));

    لكل (بنانشان والد_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      إذا (والد_idx == clone_idx) والد_idx++;
      خودڪار& والد = vertices_[والد_idx];
      بنانشان ڳڻپ = 0;
      بنانشان انگ_real = والد.obj().real_links.ڊيگهائي;
      لكل (خودڪار& l : والد.all_links_writer ())
      {
        ڳڻپ++;
        إذا (l.objidx != ٻار_idx)
          استمر;

        TRY(reassign_link (l, والد_idx, clone_idx, ڳڻپ > انگ_real));
      }
    }

    أرجع Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  گراف_نتيجو_t<بنانشان> new_ڳنڍ (حرف* سر_2, حرف* پڇ)
  {
    إذا (unlikely (vertices_.ڊيگهائي >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.ڊيگهائي);
      أرجع Err(حد_بندي_EXCEEDED);
    }

    positions_invalid = سچ;
    مفاصلو_invalid = سچ;

    خودڪار* clone = vertices_.push ();
    TRY (گراف_نتيجو_t<فراغ>::from (vertices_, ورهاست_FAILURE));

    بنانشان clone_idx = vertices_.ڊيگهائي - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتيجو_t<فراغ>::from (ordering_, ورهاست_FAILURE));

    clone->سيٽ_بفر (سر_2, پڇ);
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
  گراف_نتيجو_t<بنانشان> remap_ٻار (بنانشان والد_idx, بنانشان old_ٻار_idx)
  {
    إذا (unlikely (والد_idx >= vertices_.ڊيگهائي || old_ٻار_idx >= vertices_.ڊيگهائي))
      أرجع Err(ٻاهريون_جو_BOUNDS);

    TRY_سونپيو (بنانشان new_ٻار_idx, duplicate (old_ٻار_idx));

    خودڪار& والد = vertices_[والد_idx];
    لكل (خودڪار& l : والد.real_links_writer ())
    {
      إذا (l.objidx != old_ٻار_idx)
        استمر;
      TRY(reassign_link (l, والد_idx, new_ٻار_idx, خطأ));
    }

    لكل (خودڪار& l : والد.virtual_links_writer ())
    {
      إذا (l.objidx != old_ٻار_idx)
        استمر;
      TRY(reassign_link (l, والد_idx, new_ٻار_idx, سچ));
    }
    أرجع Ok(new_ٻار_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  گراف_نتيجو_t<منطقي> raise_childrens_ترجيح (بنانشان والد_idx)
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "  Raising priority of all children of %u",
               والد_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    إذا (unlikely (والد_idx >= vertices_.ڊيگهائي)) أرجع Err(ٻاهريون_جو_BOUNDS);
    منطقي made_change = خطأ;
    لكل (خودڪار& l : vertices_[والد_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_ترجيح ();
    أرجع Ok(made_change);
  }

  گراف_نتيجو_t<فراغ> is_fully_connected ()
  {
    TRY(update_parents());

    إذا (unlikely (پاڙ().incoming_ڪنارا_2 ()))
      // Root cannot have parents.
      أرجع Err(ORPHANED_NODES);

    لكل (بنانشان i = 0; i < پاڙ_idx (); i++)
    {
      إذا (unlikely (!vertices_[i].incoming_ڪنارا_2 ()))
        أرجع Err(ORPHANED_NODES);
    }
    أرجع Ok();
  }

#إذا 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  فراغ save_fuzzer_ٻج (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((فراغ*) &ٽيگ_2, حجم (ٽيگ_2), 1, f);

    uint16_t انگ_objects = vertices_.ڊيگهائي;
    fwrite ((فراغ*) &انگ_objects, حجم (انگ_objects), 1, f);

    لكل (ثابت خودڪار& v : vertices_)
    {
      uint16_t blob_سائيز = v.جدول_سائيز ();
      fwrite ((فراغ*) &blob_سائيز, حجم (blob_سائيز), 1, f);
      fwrite ((ثابت فراغ*) v.obj.سر_2, blob_سائيز, 1, f);
    }

    uint16_t link_ڳڻپ = 0;
    لكل (ثابت خودڪار& v : vertices_)
      link_ڳڻپ += v.obj.real_links.ڊيگهائي;

    fwrite ((فراغ*) &link_ڳڻپ, حجم (link_ڳڻپ), 1, f);

    تعريفنوع بنية
    {
      uint16_t والد;
      uint16_t ٻار;
      uint16_t position;
      uint8_t ويڪر;
    } link_t;

    لكل (بنانشان i = 0; i < vertices_.ڊيگهائي; i++)
    {
      لكل (ثابت خودڪار& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.ويڪر
        };
        fwrite ((فراغ*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#جيڪڏهن_پورو

  بنانشان انگ_roots_for_space_2 (بنانشان space) ثابت
  {
    أرجع انگ_roots_for_space[space];
  }

  بنانشان ايندڙ_space () ثابت
  {
    أرجع انگ_roots_for_space.ڊيگهائي;
  }

  گراف_نتيجو_t<فراغ> move_منزل_new_space (ثابت hb_سيٽ_t& indices)
  {
    إذا (unlikely (انگ_roots_for_space.ڊيگهائي >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 انگ_roots_for_space.ڊيگهائي);
      أرجع Err(حد_بندي_EXCEEDED);
    }
    انگ_roots_for_space.push (0);
    TRY (گراف_نتيجو_t<فراغ>::from (انگ_roots_for_space, ورهاست_FAILURE));
    بنانشان new_space = انگ_roots_for_space.ڊيگهائي - 1;

    لكل (بنانشان اشاريو_2 : indices) {
      إذا (unlikely (اشاريو_2 >= vertices_.ڊيگهائي))
        أرجع Err(ٻاهريون_جو_BOUNDS);
      خودڪار& node = vertices_[اشاريو_2];
      انگ_roots_for_space[node.space] = انگ_roots_for_space[node.space] - 1;
      انگ_roots_for_space[new_space] = انگ_roots_for_space[new_space] + 1;
      node.space = new_space;
      مفاصلو_invalid = سچ;
      positions_invalid = سچ;
    }
    أرجع Ok();
  }

  بنانشان space_for (بنانشان اشاريو_2, بنانشان* پاڙ = خالي_اشارو) ثابت
  {
    طالما (سچ)
    {
      أكد (اشاريو_2 < vertices_.ڊيگهائي);
      ثابت خودڪار& node = vertices_[اشاريو_2];
      إذا (node.space)
      {
        إذا (پاڙ != خالي_اشارو)
          *پاڙ = اشاريو_2;
        أرجع node.space;
      }

      إذا (!node.incoming_ڪنارا_2 ())
      {
        إذا (پاڙ)
          *پاڙ = اشاريو_2;
        أرجع 0;
      }

      اشاريو_2 = *node.parents_iter ();
    }
  }

  size_t ڪل_سائيز_اندريون_بائيٽون () ثابت {
    size_t ڪل_سائيز = 0;
    بنانشان ڳڻپ = vertices_.ڊيگهائي;
    لكل (بنانشان i = 0; i < ڳڻپ; i++) {
      size_t سائيز = vertices_.arrayZ[i].جدول_سائيز ();
      ڪل_سائيز += سائيز;
    }
    أرجع ڪل_سائيز;
  }


 خاص:

  فراغ print_orphaned_nodes ()
  {
    إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Graph is not fully connected.");

    parents_invalid = سچ;
    (فراغ) update_parents();

    إذا (پاڙ().incoming_ڪنارا_2 ()) {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Root node has incoming edges.");
    }

    لكل (بنانشان i = 0; i < vertices_.ڊيگهائي; i++)
    {
      ثابت خودڪار& v = vertices_[i];
      إذا (!v.incoming_ڪنارا_2 ())
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  بنانشان wide_parents (بنانشان ڳنڍ_idx, hb_سيٽ_t& parents) ثابت
  {
    بنانشان ڳڻپ = 0;
    لكل (بنانشان p : vertices_[ڳنڍ_idx].parents_iter ())
    {
      // Only real links can be wide
      لكل (ثابت خودڪار& l : vertices_[p].obj().real_links)
      {
        إذا (l.objidx == ڳنڍ_idx
            && (l.ويڪر == 3 || l.ويڪر == 4)
            && !l.is_signed)
        {
          ڳڻپ++;
          parents.جوڙيو (p);
        }
      }
    }
    أرجع ڳڻپ;
  }

 عام:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  گراف_نتيجو_t<فراغ> update_parents ()
  {
    إذا (!parents_invalid) أرجع Ok();

    بنانشان ڳڻپ = vertices_.ڊيگهائي;

    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      vertices_.arrayZ[i].reset_parents ();

    لكل (بنانشان p = 0; p < ڳڻپ; p++)
    {
      لكل (ثابت خودڪار& l : vertices_.arrayZ[p].obj().real_links)
      {
        إذا (unlikely (l.objidx >= ڳڻپ)) أرجع Err(INVALID_دليل);
        TRY(vertices_[l.objidx].جوڙيو_والد (p, خطأ));
      }

      لكل (ثابت خودڪار& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        إذا (unlikely (l.objidx >= ڳڻپ)) أرجع Err(INVALID_دليل);
        TRY(vertices_[l.objidx].جوڙيو_والد (p, سچ));
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

    بنانشان current_pos = 0;
    لكل (بنانشان i : ordering_)
    {
      خودڪار& v = vertices_[i];
      v.شروعات = current_pos;
      current_pos += v.obj().پڇ - v.obj().سر_2;
      v.پڇاڙي = current_pos;
    }

    positions_invalid = خطأ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  گراف_نتيجو_t<فراغ> update_distances ()
  {
    إذا (!مفاصلو_invalid) أرجع Ok();

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
    بنانشان ڳڻپ = vertices_.ڊيگهائي;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[پاڙ_idx ()].distance = 0;

    hb_ترجيح_قطار_t<int64_t> قطار;
    قطار.alloc (ڳڻپ);
    قطار.insert (0, پاڙ_idx ());
    TRY (گراف_نتيجو_t<فراغ>::from (قطار, ورهاست_FAILURE));

    hb_ويڪٽر_t<منطقي> گهميل;
    گهميل.resize (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (گهميل, ورهاست_FAILURE));

    طالما (!قطار.اندريون_error () && !قطار.is_empty ())
    {
      بنانشان ايندڙ_idx = قطار.ڪڍو_minimum ().second;
      إذا (گهميل[ايندڙ_idx]) استمر;
      ثابت خودڪار& next = vertices_[ايندڙ_idx];
      int64_t ايندڙ_مفاصلو = next.distance;
      گهميل[ايندڙ_idx] = سچ;

      لكل (ثابت خودڪار& link : next.obj().all_links ())
      {
        إذا (unlikely (link.objidx >= vertices_.ڊيگهائي))
          أرجع Err(ٻاهريون_جو_BOUNDS);

        إذا (گهميل[link.objidx]) استمر;

        خودڪار& ٻار_v = vertices_.arrayZ[link.objidx];
        ثابت خودڪار& ٻار = ٻار_v.obj ();
        بنانشان link_ويڪر = link.ويڪر ? link.ويڪر : 4; // treat virtual offsets as 32 bits wide
        int64_t ٻار_وزن = (ٻار.پڇ - ٻار.سر_2) +
                               ((int64_t) 1 << (link_ويڪر * 8)) * (ٻار_v.space + 1);
        int64_t ٻار_مفاصلو = ايندڙ_مفاصلو + ٻار_وزن;

        إذا (ٻار_مفاصلو < ٻار_v.distance)
        {
          ٻار_v.distance = ٻار_مفاصلو;
          قطار.insert (ٻار_مفاصلو, link.objidx);
        }
      }
    }

    TRY (گراف_نتيجو_t<فراغ>::from (قطار, ورهاست_FAILURE));
    إذا (unlikely (!قطار.is_empty ()))
    {
      print_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    مفاصلو_invalid = خطأ;
    أرجع Ok();
  }

 خاص:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  گراف_نتيجو_t<فراغ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      بنانشان والد_idx,
                      بنانشان new_idx,
                      منطقي is_virtual)
  {
    بنانشان old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_والد (والد_idx);
    أرجع vertices_[new_idx].جوڙيو_والد (والد_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  سانچو<قسم_نالو Iterator, hb_requires (hb_is_iterator (Iterator))>
  گراف_نتيجو_t<فراغ> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقي only_wide = خطأ)
  {
    إذا (!id_map) أرجع Ok();
    لكل (بنانشان i : subgraph)
    {
      خودڪار& راس = vertices_[i];
      بنانشان انگ_real = راس.obj().real_links.ڊيگهائي;
      بنانشان ڳڻپ = 0;
      لكل (خودڪار& link : راس.all_links_writer ())
      {
        ڳڻپ++;
        ثابت uint32_t *v;
        إذا (!id_map.has (link.objidx, &v)) استمر;
        إذا (only_wide && (link.is_signed || (link.ويڪر != 4 && link.ويڪر != 3))) استمر;

        TRY(reassign_link (link, i, *v, ڳڻپ > انگ_real));
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
  گراف_نتيجو_t<فراغ> ڳوليو_connected_nodes (بنانشان شروعات_idx,
                                             hb_سيٽ_t& targets,
                                             hb_سيٽ_t& گهميل,
                                             hb_سيٽ_t& connected)
  {
    TRY (گراف_نتيجو_t<فراغ>::from (گهميل, ورهاست_FAILURE));
    إذا (گهميل.has (شروعات_idx)) أرجع Ok();
    إذا (unlikely (شروعات_idx >= vertices_.ڊيگهائي))
    {
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      أرجع Err(ٻاهريون_جو_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.ڊيگهائي);
    TRY (گراف_نتيجو_t<فراغ>::from (ordering_scratch_, ورهاست_FAILURE));

    خودڪار& ڍير = ordering_scratch_;
    بنانشان ڍير_len = 0;

    گراف_نتيجو_t<فراغ> res = Ok();
    خودڪار handle_ڳنڍ = [&] (بنانشان ڳنڍ_idx) {
      گهميل.جوڙيو (ڳنڍ_idx);
      إذا (targets.has (ڳنڍ_idx)) {
        targets.del (ڳنڍ_idx);
        connected.جوڙيو (ڳنڍ_idx);
      }

      إذا (unlikely (ڍير_len >= ڍير.ڊيگهائي))
      {
        res = Err(چڪر_DETECTED);
        أرجع خطأ;
      }
      ڍير[ڍير_len++] = ڳنڍ_idx;
      أرجع سچ;
    };

    إذا (!handle_ڳنڍ (شروعات_idx)) أرجع res;

    طالما (ڍير_len > 0)
    {
      بنانشان ڳنڍ_idx = ڍير[--ڍير_len];
      ثابت خودڪار& v = vertices_[ڳنڍ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      لكل (ثابت خودڪار& l : v.obj().all_links ())
      {
        بنانشان ٻار_idx = l.objidx;
        إذا (گهميل.has (ٻار_idx)) استمر;
        إذا (!handle_ڳنڍ (ٻار_idx)) أرجع res;
      }

      لكل (بنانشان والد_idx : v.parents_iter ())
      {
        إذا (گهميل.has (والد_idx)) استمر;
        إذا (!handle_ڳنڍ (والد_idx)) أرجع res;
      }
    }

    TRY (گراف_نتيجو_t<فراغ>::from (گهميل, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (connected, ورهاست_FAILURE));

    أرجع Ok();
  }

 عام:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_ويڪٽر_t<راس_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_ويڪٽر_t<بنانشان> ordering_;
  hb_ويڪٽر_t<بنانشان> ordering_scratch_;

 خاص:
  منطقي parents_invalid = سچ;
  منطقي مفاصلو_invalid = سچ;
  منطقي positions_invalid = سچ;
  hb_ويڪٽر_t<بنانشان> انگ_roots_for_space;
  hb_ويڪٽر_t<حرف*> buffers;
};

}

#جيڪڏهن_پورو  // GRAPH_GRAPH_HH
