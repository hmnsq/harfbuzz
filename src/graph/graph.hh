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

#جے_بنا_تعریف گراف_گراف_HH
#تعریف_کرو گراف_گراف_HH

#تعریف_کرو HB_گراف_INVALID ((بنانشانی) -1)

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
    بنانشانی space = 0 ;
    بنانشانی شروع = 0;
    بنانشانی سرا = 0;
    بنانشانی ترجیح = 0;

    خاص:
    hb_serialize_context_t::object_t obj_;
    بنانشانی incoming_کنارے = 0;
    بنانشانی single_ماپا = HB_گراف_INVALID;
    منطقي has_incoming_virtual_کنارے = خطأ;
    hb_hashmap_t<بنانشانی, بنانشانی> parents;
    hb_سیٹ_t virtual_parents;
    عام:

    راس_t ()
    {}

    واضح راس_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    خودکار parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_ماپا, single_ماپا != HB_گراف_INVALID),
	parents.کلیدیں_ref ()
      )
    )

    فراغ سیٹ_بفر (حرف* سر_2, حرف* پوچھ) {
      obj_.سر_2 = سر_2;
      obj_.پوچھ = پوچھ;
    }

    فراغ shrink_بفر (size_t گنتی)
    {
      obj_.پوچھ -= گنتی;
    }

    خودکار all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    خودکار real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    خودکار virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    فراغ ترتیب_دیو_real_links () {
      obj_.real_links.qsort ();
    }

    گراف_نتیجہ_t<فراغ> جوڑو_real_link (بنانشانی چوڑائی, بنانشانی obj_idx, بنانشانی position) {
      خودکار* link = obj_.real_links.push ();
      TRY (گراف_نتیجہ_t<فراغ>::from (obj_.real_links, ونڈ_FAILURE));
      link->چوڑائی = چوڑائی;
      link->objidx = obj_idx;
      link->position = position;
      أرجع Ok();
    }

    گراف_نتیجہ_t<فراغ> جوڑو_virtual_link (بنانشانی obj_idx) {
      خودکار* link = obj_.virtual_links.push ();
      TRY (گراف_نتیجہ_t<فراغ>::from (obj_.virtual_links, ونڈ_FAILURE));
      link->objidx = obj_idx;
      أرجع Ok();
    }

    گراف_نتیجہ_t<فراغ> جوڑو_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      أرجع گراف_نتیجہ_t<فراغ>::from (obj_.real_links, ونڈ_FAILURE);
    }

    گراف_نتیجہ_t<فراغ> سیٹ_real_links (hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(گراف_نتیجہ_t<فراغ>::from (links, ونڈ_FAILURE));
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

    گراف_نتیجہ_t<فراغ> جوڑو_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      أرجع گراف_نتیجہ_t<فراغ>::from (obj_.virtual_links, ونڈ_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { أرجع obj_; }

    سانچا<قسم_ناں T>
    T as () ثابت
    {
      أرجع reinterpret_cast<T> (obj_.سر_2);
    }

    منطقي has_incoming_virtual_کنارے_2 () ثابت
    {
      أرجع has_incoming_virtual_کنارے;
    }

    گراف_نتیجہ_t<فراغ> link_positions_valid (بنانشانی عدد_objects, منطقي removed_nil)
    {
      hb_سیٹ_t assigned_بائٹاں;
      لكل (ثابت خودکار& l : obj_.real_links)
      {
        إذا (unlikely (l.objidx >= عدد_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                     "Invalid graph. Invalid object index.");
          أرجع Err(INVALID_دلیل);
        }

        بنانشانی شروع = l.position;
        بنانشانی سرا = شروع + l.چوڑائی - 1;

        إذا (unlikely (l.چوڑائی < 2 || l.چوڑائی > 4))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                     "Invalid graph. Invalid link width.");
          أرجع Err(INVALID_دلیل);
        }

        إذا (unlikely (سرا >= جدول_ناپ ()))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                     "Invalid graph. Link position is out of bounds.");
          أرجع Err(باہرلا_دا_BOUNDS);
        }

        إذا (unlikely (assigned_بائٹاں.intersects (شروع, سرا)))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                     "Invalid graph. Found offsets whose positions overlap.");
          أرجع Err(INVALID_دلیل);
        }

        assigned_بائٹاں.جوڑو_range (شروع, سرا);
      }

      أرجع گراف_نتیجہ_t<فراغ>::from (assigned_بائٹاں, ونڈ_FAILURE);
    }

    فراغ normalize ()
    {
      obj_.real_links.qsort ();
      لكل (ثابت خودکار& l : obj_.real_links)
      {
        لكل (بنانشانی i = 0; i < l.چوڑائی; i++)
        {
          obj_.سر_2[l.position + i] = 0;
        }
      }
    }

    منطقي equals (بنانشانی this_اشاریہ,
                 بنانشانی other_اشاریہ,
                 ثابت راس_t& other,
                 ثابت گراف_t& گراف,
                 ثابت گراف_t& other_گراف,
                 بنانشانی ڈونگھائی) ثابت
    {
      إذا (!(as_بائٹاں () == other.as_بائٹاں ()))
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_اشاریہ,
                   (بنانشانی لما) جدول_ناپ (),
                   other_اشاریہ,
                   (بنانشانی لما) other.جدول_ناپ (),
                   ڈونگھائی);

        خودکار a = as_بائٹاں ();
        خودکار b = other.as_بائٹاں ();
        طالما (a || b)
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                     "  0x%x %s 0x%x", (بنانشانی) *a, (*a == *b) ? "==" : "!=", (بنانشانی) *b);
          a++;
          b++;
        }
        أرجع خطأ;
      }

      أرجع links_equal (obj_.real_links, other.obj_.real_links, گراف, other_گراف, ڈونگھائی);
    }

    hb_بائٹاں_t as_بائٹاں () ثابت
    {
      أرجع hb_بائٹاں_t (obj_.سر_2, جدول_ناپ ());
    }

    یار فراغ swap (راس_t& a, راس_t& b)
    {
      hb_وٹاندرا (a.obj_, b.obj_);
      hb_وٹاندرا (a.distance, b.distance);
      hb_وٹاندرا (a.space, b.space);
      hb_وٹاندرا (a.single_ماپا, b.single_ماپا);
      hb_وٹاندرا (a.parents, b.parents);
      hb_وٹاندرا (a.virtual_parents, b.virtual_parents);
      hb_وٹاندرا (a.incoming_کنارے, b.incoming_کنارے);
      hb_وٹاندرا (a.has_incoming_virtual_کنارے, b.has_incoming_virtual_کنارے);
      hb_وٹاندرا (a.شروع, b.شروع);
      hb_وٹاندرا (a.سرا, b.سرا);
      hb_وٹاندرا (a.ترجیح, b.ترجیح);
    }

    گراف_نتیجہ_t<hb_hashmap_t<بنانشانی, بنانشانی>>
    position_منزل_اشاریہ_map () ثابت
    {
      hb_hashmap_t<بنانشانی, بنانشانی> نتیجہ;

      نتیجہ.alloc (obj_.real_links.لمائی);
      لكل (ثابت خودکار& l : obj_.real_links) {
        نتیجہ.سیٹ (l.position, l.objidx);
      }

      أرجع گراف_نتیجہ_t<hb_hashmap_t<بنانشانی, بنانشانی>>::from (std::move(نتیجہ), ونڈ_FAILURE);
    }

    منطقي is_shared () ثابت
    {
      أرجع parents.get_population () > virtual_parents.get_population () + 1;
    }

    بنانشانی incoming_کنارے_2 () ثابت
    {
      إذا (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_کنارے == (single_ماپا != HB_گراف_INVALID) +
		(parents.اقدار_ref () | hb_reduce (hb_جوڑو, 0)));
       }
      أرجع incoming_کنارے;
    }

    بنانشانی incoming_کنارے_from_ماپا (بنانشانی ماپا_اشاریہ) ثابت {
      إذا (single_ماپا != HB_گراف_INVALID) {
        أرجع single_ماپا == ماپا_اشاریہ ? 1 : 0;
      }

      بنانشانی* گنتی;
      أرجع  parents.has(ماپا_اشاریہ, &گنتی) ? *گنتی : 0;
    }

    فراغ reset_parents ()
    {
      incoming_کنارے = 0;
      has_incoming_virtual_کنارے = خطأ;
      single_ماپا = HB_گراف_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    گراف_نتیجہ_t<فراغ> جوڑو_ماپا (بنانشانی ماپا_اشاریہ, منطقي is_virtual)
    {
      أكد (ماپا_اشاریہ != HB_گراف_INVALID);
      has_incoming_virtual_کنارے |= is_virtual;
      إذا (is_virtual)
        virtual_parents.جوڑو (ماپا_اشاریہ);

      إذا (incoming_کنارے == 0)
      {
	single_ماپا = ماپا_اشاریہ;
	incoming_کنارے = 1;
	أرجع منزل_نتیجہ();
      }
      وإلا إذا (single_ماپا != HB_گراف_INVALID)
      {
        أكد (incoming_کنارے == 1);
	إذا (!parents.سیٹ (single_ماپا, 1))
	  أرجع منزل_نتیجہ ();
	single_ماپا = HB_گراف_INVALID;
      }

      بنانشانی *v;
      إذا (parents.has (ماپا_اشاریہ, &v))
      {
        (*v)++;
	incoming_کنارے++;
      }
      وإلا إذا (parents.سیٹ (ماپا_اشاریہ, 1))
	incoming_کنارے++;

      أرجع منزل_نتیجہ();
    }

    فراغ remove_ماپا (بنانشانی ماپا_اشاریہ)
    {
      إذا (ماپا_اشاریہ == single_ماپا)
      {
	single_ماپا = HB_گراف_INVALID;
	incoming_کنارے--;
	virtual_parents.reset ();
	أرجع;
      }

      بنانشانی *v;
      إذا (parents.has (ماپا_اشاریہ, &v))
      {
	incoming_کنارے--;
	إذا (*v > 1)
	  (*v)--;
	وإلا
        {
	  parents.del (ماپا_اشاریہ);
          virtual_parents.del (ماپا_اشاریہ);
        }

	إذا (incoming_کنارے == 1)
	{
	  single_ماپا = *parents.کلیدیں ();
	  parents.reset ();
	}
      }
    }

    فراغ remove_real_link_unordered (بنانشانی link_array_اشاریہ) {
      obj_.real_links.remove_unordered (link_array_اشاریہ);
    }

    فراغ remove_real_link_unordered (بنانشانی بچہ_اشاریہ, ثابت فراغ* وتھ)
    {
      بنانشانی گنتی = obj_.real_links.لمائی;
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
        خودکار& link = obj_.real_links.arrayZ[i];
        إذا (link.objidx != بچہ_اشاریہ)
          استمر;

        إذا ((obj_.سر_2 + link.position) != وتھ)
          استمر;

        obj_.real_links.remove_unordered (i);
        أرجع;
      }
    }

    گراف_نتیجہ_t<فراغ> remap_ماپا (بنانشانی old_اشاریہ, بنانشانی new_اشاریہ)
    {
      إذا (single_ماپا != HB_گراف_INVALID)
      {
        إذا (single_ماپا == old_اشاریہ)
        {
	  single_ماپا = new_اشاریہ;
          إذا (virtual_parents.has (old_اشاریہ))
          {
            virtual_parents.del (old_اشاریہ);
            virtual_parents.جوڑو (new_اشاریہ);
          }
        }
        أرجع گراف_نتیجہ_t<فراغ>::from(virtual_parents, ونڈ_FAILURE);
      }

      ثابت بنانشانی *pv;
      إذا (parents.has (old_اشاریہ, &pv))
      {
        بنانشانی v = *pv;
	إذا (!parents.سیٹ (new_اشاریہ, v))
          incoming_کنارے -= v;
	parents.del (old_اشاریہ);

        إذا (incoming_کنارے == 1)
	{
	  single_ماپا = *parents.کلیدیں ();
	  parents.reset ();
	}
      }

      إذا (virtual_parents.has (old_اشاریہ))
      {
        virtual_parents.del (old_اشاریہ);
        virtual_parents.جوڑو (new_اشاریہ);
      }

      أرجع گراف_نتیجہ_t<فراغ>::from(virtual_parents, ونڈ_FAILURE);
    }

    منطقي is_پتی () ثابت
    {
      أرجع !obj_.real_links.لمائی && !obj_.virtual_links.لمائی;
    }

    منطقي raise_ترجیح ()
    {
      إذا (has_max_ترجیح ()) أرجع خطأ;
      ترجیح++;
      أرجع سچ;
    }

    منطقي give_max_ترجیح ()
    {
      منطقي نتیجہ = خطأ;
      طالما (!has_max_ترجیح()) {
        نتیجہ = سچ;
        ترجیح++;
      }
      أرجع نتیجہ;
    }

    منطقي has_max_ترجیح () ثابت {
      أرجع ترجیح >= 3;
    }

    size_t جدول_ناپ () ثابت {
      أرجع obj_.پوچھ - obj_.سر_2;
    }

    int64_t modified_فاصلہ (بنانشانی ترتیب) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_فاصلہ =
          hb_clamp (distance + فاصلہ_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      إذا (has_max_ترجیح ()) {
        modified_فاصلہ = 0;
      }
      أرجع (modified_فاصلہ << 18) | (0x003FFFF & ترتیب);
    }

    int64_t فاصلہ_modifier () ثابت
    {
      إذا (!ترجیح) أرجع 0;
      int64_t جدول_ناپ = obj_.پوچھ - obj_.سر_2;

      إذا (ترجیح == 1)
        أرجع -جدول_ناپ / 2;

      أرجع -جدول_ناپ;
    }

   خاص:
    گراف_نتیجہ_t<فراغ> منزل_نتیجہ() ثابت {
      إذا (unlikely (
        parents.وچ_error() || virtual_parents.وچ_error() ||
        obj_.real_links.وچ_error() || obj_.virtual_links.وچ_error()))
        أرجع Err(ونڈ_FAILURE);
      أرجع Ok();
    }

    منطقي links_equal (ثابت hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت گراف_t& گراف,
                      ثابت گراف_t& other_گراف,
                      بنانشانی ڈونگھائی) ثابت
    {
      خودکار a = this_links.iter ();
      خودکار b = other_links.iter ();

      طالما (a && b)
      {
        ثابت خودکار& link_a = *a;
        ثابت خودکار& link_b = *b;

        إذا (link_a.چوڑائی != link_b.چوڑائی ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          أرجع خطأ;

        إذا (!گراف.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_گراف.vertices_[link_b.objidx], گراف, other_گراف, ڈونگھائی + 1))
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

  سانچا <قسم_ناں T, راس_mutability_t mutability = Mutable>
  بنية راس_and_جدول_t
  {
    راس_and_جدول_t () : اشاریہ_2 (0), راس (خالی_سوچک), جدول (خالی_سوچک)
    {}

    سانچا <راس_mutability_t other_mutability>
    راس_and_جدول_t (ثابت راس_and_جدول_t<T, other_mutability>& o)
      : اشاریہ_2 (o.اشاریہ_2), راس (o.راس), جدول (o.جدول)
    {}

    بنانشانی اشاریہ_2;
    قسم_ناں std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type راس;
    قسم_ناں std::conditional<mutability == Immutable, ثابت T*, T*>::type جدول;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  سانچا<قسم_ناں T>
  ساكن گراف_نتیجہ_t<گراف_t> create (ثابت T& objects)
  {
    إذا (unlikely (objects.لمائی > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.لمائی);
      أرجع Err(حد_بندی_EXCEEDED);
    }

    گراف_t g;
    g.عدد_roots_for_space.push (1);
    TRY (گراف_نتیجہ_t<فراغ>::from (g.عدد_roots_for_space, ونڈ_FAILURE));

    منطقي removed_nil = خطأ;
    ثابت بنانشانی گنتی = objects.لمائی;
    g.vertices_.alloc (گنتی);
    g.ordering_.resize (گنتی);
    g.ordering_scratch_.alloc (گنتی);
    TRY (گراف_نتیجہ_t<فراغ>::from (g.vertices_, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (g.ordering_, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (g.ordering_scratch_, ونڈ_FAILURE));

    بنانشانی ترتیب = objects.لمائی;
    بنانشانی skip = 0;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      إذا (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = سچ;
        ترتیب--;
        g.ordering_.resize (گنتی - 1);
        skip++;
        استمر;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      إذا (unlikely (obj.وچ_error ())) {
        أرجع Err(ونڈ_FAILURE);
      }
      راس_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (گنتی, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      بنانشانی obj_idx = i - skip;
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

  گراف_t (ثابت گراف_t&) = مٹاؤ;
  گراف_t& عامل = (ثابت گراف_t&) = مٹاؤ;
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
    أرجع جڑ ().equals (جڑ_idx(), other.جڑ_idx(), other.جڑ (), *هذا, other, 0);
  }

  فراغ print () ثابت {
    لكل (بنانشانی id : ordering_)
    {
      ثابت خودکار& v = vertices_[id];
      لکھو("%u: %u [", id, (بنانشانی صحيح)v.جدول_ناپ());
      لكل (ثابت خودکار &l : v.obj().real_links) {
        لکھو("%u, ", l.objidx);
      }
      لكل (ثابت خودکار &l : v.obj().virtual_links) {
        لکھو("v%u, ", l.objidx);
      }
      لکھو("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  فراغ normalize ()
  {
    لكل (خودکار& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت راس_t& جڑ () ثابت
  {
    أرجع vertices_[جڑ_idx ()];
  }

  بنانشانی جڑ_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    أرجع ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (بنانشانی i) ثابت
  {
    أرجع vertices_[i].obj();
  }

  گراف_نتیجہ_t<فراغ> جوڑو_بفر (حرف* بفر)
  {
    buffers.push (بفر);
    أرجع گراف_نتیجہ_t<فراغ>::from (buffers, ونڈ_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  سانچا<قسم_ناں T>
  گراف_نتیجہ_t<فراغ> جوڑو_link (T* وتھ,
                                 بنانشانی ماپا_id,
                                 بنانشانی بچہ_id)
  {
    إذا (unlikely (ماپا_id >= vertices_.لمائی || بچہ_id >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);
    خودکار& v = vertices_[ماپا_id];
    TRY(v.جوڑو_real_link(2, بچہ_id, (حرف*) وتھ - (حرف*) v.obj().سر_2));
    أرجع vertices_[بچہ_id].جوڑو_ماپا (ماپا_id, خطأ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  گراف_نتیجہ_t<فراغ> ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ_if_needed ()
  {
    إذا (!positions_invalid) أرجع Ok();
    أرجع ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  گراف_نتیجہ_t<فراغ> ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ()
  {
    positions_invalid = سچ;

    إذا (vertices_.لمائی <= 1) {
      // Graph of 1 or less doesn't need sorting.
      أرجع Ok();
    }

    TRY (update_distances ());

    hb_ترجیح_قطار_t<int64_t> قطار;
    قطار.alloc (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (قطار, ونڈ_FAILURE));

    hb_سمتیہ_t<بنانشانی> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (new_ordering, ونڈ_FAILURE));

    hb_سمتیہ_t<بنانشانی> removed_کنارے;
    removed_کنارے.resize (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (removed_کنارے, ونڈ_FAILURE));

    TRY (update_parents ());

    قطار.insert (جڑ ().modified_فاصلہ (0), جڑ_idx ());
    بنانشانی ترتیب = 1;
    بنانشانی pos = 0;
    طالما (!قطار.وچ_error () && !قطار.is_empty ())
    {
      بنانشانی اگلا_id = قطار.نکالو_minimum().second;

      إذا (unlikely (pos >= new_ordering.لمائی)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Invalid graph. Contains cycle.");
        أرجع Err(چکر_DETECTED);
      }
      new_ordering[pos++] = اگلا_id;
      ثابت راس_t& next = vertices_[اگلا_id];

      لكل (ثابت خودکار& link : next.obj().all_links ()) {
        إذا (unlikely (link.objidx >= vertices_.لمائی))
          أرجع Err(باہرلا_دا_BOUNDS);
        removed_کنارے[link.objidx]++;
        ثابت خودکار& v = vertices_[link.objidx];
        إذا (!(v.incoming_کنارے_2 () - removed_کنارے[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          قطار.insert (v.modified_فاصلہ (ترتیب++),
                        link.objidx);
      }
    }

    TRY (گراف_نتیجہ_t<فراغ>::from (قطار, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (new_ordering, ونڈ_FAILURE));

    hb_وٹاندرا (ordering_, new_ordering);

    إذا (unlikely (pos != vertices_.لمائی)) {
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
  گراف_نتیجہ_t<فراغ> لبھو_space_roots (hb_سیٹ_t& ویکھیا, hb_سیٹ_t& roots)
  {
    بنانشانی جڑ_اشاریہ = جڑ_idx ();
    لكل (بنانشانی i : ordering_)
    {
      إذا (ویکھیا.has (i)) استمر;

      // Only real links can form 32 bit spaces
      لكل (ثابت خودکار& l : vertices_[i].obj().real_links)
      {
        إذا (l.is_signed || l.چوڑائی < 3)
          استمر;

        إذا (i == جڑ_اشاریہ && l.چوڑائی == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          استمر;

        إذا (l.چوڑائی >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_سیٹ_t sub_roots;
          TRY (لبھو_32bit_roots (l.objidx, sub_roots));
          إذا (sub_roots) {
            لكل (بنانشانی sub_جڑ_idx : sub_roots) {
              roots.جوڑو (sub_جڑ_idx);
              TRY (لبھو_subgraph (sub_جڑ_idx, ویکھیا));
            }
            استمر;
          }
        }

        roots.جوڑو (l.objidx);
        TRY (لبھو_subgraph (l.objidx, ویکھیا));
      }
    }

    TRY(گراف_نتیجہ_t<فراغ>::from (ویکھیا, ونڈ_FAILURE));
    TRY(گراف_نتیجہ_t<فراغ>::from (roots, ونڈ_FAILURE));
    أرجع Ok();
  }

  سانچا <قسم_ناں T, قسم_ناں ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, Immutable>> as_جدول (بنانشانی ماپا, ثابت فراغ* وتھ, Ts... ds)
  {
    TRY_سونپو (بنانشانی idx, اشاریہ_for_وتھ (ماپا, وتھ));
    أرجع as_جدول_from_اشاریہ<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچا <قسم_ناں T, قسم_ناں ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, Mutable>> as_mutable_جدول (بنانشانی ماپا, ثابت فراغ* وتھ, Ts... ds)
  {
    TRY_سونپو (خودکار idx, mutable_اشاریہ_for_وتھ (ماپا, وتھ));
    أرجع as_جدول_from_اشاریہ<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچا <قسم_ناں T, راس_mutability_t mutability = Mutable, قسم_ناں ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, mutability>> as_جدول_from_اشاریہ (بنانشانی اشاریہ_2, Ts... ds)
  {
    إذا (unlikely (اشاریہ_2 >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    راس_and_جدول_t<T, mutability> r;
    r.راس = (قسم_ناں std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type) &vertices_[اشاریہ_2];
    r.جدول = r.راس->سانچا as<قسم_ناں std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.اشاریہ_2 = اشاریہ_2;
    إذا (unlikely (!r.جدول))
      أرجع Err(INVALID_دلیل);

    TRY(r.جدول->sanitize (*(r.راس), std::forward<Ts>(ds)...));
    أرجع r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  گراف_نتیجہ_t<بنانشانی> اشاریہ_for_وتھ (بنانشانی گرہ_idx, ثابت فراغ* وتھ) ثابت
  {
    إذا (unlikely (گرہ_idx >= vertices_.لمائی)) أرجع Err(باہرلا_دا_BOUNDS);
    ثابت خودکار& node = object (گرہ_idx);
    إذا (unlikely (وتھ < node.سر_2 || وتھ >= node.پوچھ)) أرجع Err(باہرلا_دا_BOUNDS);

    بنانشانی گنتی = node.real_links.لمائی;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت خودکار& link = node.real_links.arrayZ[i];
      إذا (وتھ != node.سر_2 + link.position)
        استمر;
      أرجع Ok(link.objidx);
    }

    أرجع Err(INVALID_دلیل);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  گراف_نتیجہ_t<بنانشانی> mutable_اشاریہ_for_وتھ (بنانشانی گرہ_idx, ثابت فراغ* وتھ)
  {
    TRY_سونپو (بنانشانی بچہ_idx, اشاریہ_for_وتھ (گرہ_idx, وتھ));
    إذا (unlikely (بچہ_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    خودکار& بچہ = vertices_[بچہ_idx];
    لكل (بنانشانی p : بچہ.parents_iter ())
    {
      إذا (p != گرہ_idx) {
        أرجع duplicate (گرہ_idx, بچہ_idx, سچ);
      }
    }

    أرجع Ok(بچہ_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  گراف_نتیجہ_t<منطقي> سونپو_spaces ()
  {
    TRY (update_parents ());

    لكل (خودکار& راس : vertices_.writer ())
    {
      راس.space = 0;
      راس.ترجیح = 0;
    }
    عدد_roots_for_space.resize (1);
    TRY (گراف_نتیجہ_t<فراغ>::from (عدد_roots_for_space, ونڈ_FAILURE));
    عدد_roots_for_space[0] = 1;
    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    hb_سیٹ_t ویکھیا;
    hb_سیٹ_t roots;
    TRY (لبھو_space_roots (ویکھیا, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    ویکھیا.invert ();

    إذا (!roots) أرجع Ok(خطأ);

    طالما (roots)
    {
      uint32_t next = HB_سیٹ_قدر_INVALID;
      TRY (گراف_نتیجہ_t<فراغ>::from (roots, ونڈ_FAILURE));
      إذا (!roots.next (&next)) اكسر;

      hb_سیٹ_t connected_roots;
      TRY (لبھو_connected_nodes (next, roots, ویکھیا, connected_roots));
      TRY (گراف_نتیجہ_t<فراغ>::from (connected_roots, ونڈ_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (گراف_نتیجہ_t<فراغ>::from (connected_roots, ونڈ_FAILURE));

      بنانشانی اگلا_space = هذا->اگلا_space ();
      إذا (unlikely (اگلا_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 اگلا_space);
        أرجع Err(حد_بندی_EXCEEDED);
      }
      عدد_roots_for_space.push (0);
      TRY(گراف_نتیجہ_t<فراغ>::from (عدد_roots_for_space, ونڈ_FAILURE));

      لكل (بنانشانی جڑ : connected_roots)
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Subgraph %u gets space %u", جڑ, اگلا_space);
        vertices_[جڑ].space = اگلا_space;
        عدد_roots_for_space[اگلا_space] = عدد_roots_for_space[اگلا_space] + 1;
        فاصلہ_invalid = سچ;
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
  گراف_نتیجہ_t<منطقي> isolate_subgraph (hb_سیٹ_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_سیٹ_t parents;
    لكل (بنانشانی جڑ_idx : roots)
    {
      إذا (unlikely (جڑ_idx >= vertices_.لمائی))
        أرجع Err(باہرلا_دا_BOUNDS);
      subgraph.سیٹ (جڑ_idx, wide_parents (جڑ_idx, parents));
    }
    لكل (بنانشانی جڑ_idx : roots)
    {
      TRY (لبھو_subgraph (جڑ_idx, subgraph));
    }
    TRY (گراف_نتیجہ_t<فراغ>::from (subgraph, ونڈ_FAILURE));

    hb_map_t اشاریہ_map;
    منطقي made_changes = خطأ;
    لكل (خودکار اندراج : subgraph.iter ())
    {
      أكد (اندراج.پہلا < vertices_.لمائی);
      ثابت خودکار& node = vertices_[اندراج.پہلا];
      بنانشانی subgraph_incoming_کنارے = اندراج.second;

      إذا (subgraph_incoming_کنارے < node.incoming_کنارے_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = سچ;
        TRY (duplicate_subgraph (اندراج.پہلا, اشاریہ_map));
      }
    }

    TRY (گراف_نتیجہ_t<فراغ>::from (اشاریہ_map, ونڈ_FAILURE));

    إذا (!made_changes)
      أرجع Ok(خطأ);

    خودکار new_subgraph =
        + subgraph.کلیدیں ()
        | hb_map([&] (uint32_t گرہ_idx) {
          ثابت uint32_t *v;
          إذا (اشاریہ_map.has (گرہ_idx, &v)) أرجع *v;
          أرجع گرہ_idx;
        })
        ;

    TRY(remap_obj_indices (اشاریہ_map, new_subgraph));
    TRY(remap_obj_indices (اشاریہ_map, parents.iter (), سچ));

    // Update roots set with new indices as needed.
    لكل (خودکار next : roots)
    {
      ثابت uint32_t *v;
      إذا (اشاریہ_map.has (next, &v))
      {
        roots.del (next);
        roots.جوڑو (*v);
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
  سانچا <قسم_ناں ویکھو_کنارا_Func>
  گراف_نتیجہ_t<فراغ> traverse_directed_bfs (بنانشانی شروع_idx, ویکھو_کنارا_Func&& ویکھو_کنارا)
  {
    إذا (unlikely (شروع_idx >= vertices_.لمائی))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      أرجع Err(باہرلا_دا_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (ordering_scratch_, ونڈ_FAILURE));

    إذا (!ویکھو_کنارا (HB_CODEPOINT_INVALID, خالی_سوچک, شروع_idx, 0))
      أرجع Ok();

    بنانشانی سر_2 = 0;
    بنانشانی پوچھ = 0;
    خودکار& قطار = ordering_scratch_;
    قطار[پوچھ++] = شروع_idx;

    بنانشانی ڈونگھائی = 0;
    طالما (سر_2 < پوچھ)
    {
      بنانشانی level_سرا = پوچھ;
      طالما (سر_2 < level_سرا)
      {
        بنانشانی گرہ_idx = قطار[سر_2++];
        ثابت خودکار& v = vertices_[گرہ_idx];

        بنانشانی عدد_real = v.obj().real_links.لمائی;
        بنانشانی کل_links = عدد_real + v.obj().virtual_links.لمائی;

        لكل (بنانشانی i = 0; i < کل_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت خودکار& v = vertices_[گرہ_idx];
          ثابت خودکار& link = (i < عدد_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - عدد_real];
          بنانشانی بچہ_idx = link.objidx;

          إذا (!ویکھو_کنارا (گرہ_idx, &link, بچہ_idx, ڈونگھائی + 1))
            استمر;

          إذا (unlikely (پوچھ >= قطار.لمائی))
            أرجع Err(چکر_DETECTED);

          قطار[پوچھ++] = بچہ_idx;
        }
      }
      ڈونگھائی++;
    }
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> لبھو_subgraph (بنانشانی گرہ_idx, hb_map_t& subgraph)
  {
    أرجع traverse_directed_bfs (گرہ_idx, [&] (
      بنانشانی ماپا,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشانی بچہ,
      بنانشانی ڈونگھائی) {
      إذا (ڈونگھائی == 0) أرجع سچ;
      hb_codepoint_t *گنتی;
      إذا (subgraph.has (بچہ, &گنتی))
      {
        (*گنتی)++;
        أرجع خطأ;
      }
      subgraph.سیٹ (بچہ, 1);
      أرجع سچ;
    });
  }

  گراف_نتیجہ_t<فراغ> لبھو_subgraph (بنانشانی گرہ_idx, hb_سیٹ_t& subgraph)
  {
    أرجع traverse_directed_bfs (گرہ_idx, [&] (
      بنانشانی ماپا,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشانی بچہ,
      بنانشانی ڈونگھائی) {
      إذا (subgraph.has (بچہ)) أرجع خطأ;
      subgraph.جوڑو (بچہ);
      أرجع سچ;
    });
  }

  گراف_نتیجہ_t<size_t> لبھو_subgraph_ناپ (بنانشانی گرہ_idx, hb_سیٹ_t& subgraph, بنانشانی max_ڈونگھائی = HB_گراف_INVALID)
  {
    size_t ناپ = 0;
    TRY (traverse_directed_bfs (گرہ_idx, [&] (
      بنانشانی ماپا,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشانی بچہ,
      بنانشانی ڈونگھائی) {
      إذا (subgraph.has (بچہ)) أرجع خطأ;
      subgraph.جوڑو (بچہ);

      ثابت خودکار& o = vertices_[بچہ].obj();
      ناپ += o.پوچھ - o.سر_2;
      أرجع ڈونگھائی < max_ڈونگھائی;
    }));
    أرجع Ok(ناپ);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  گراف_نتیجہ_t<فراغ> لبھو_32bit_roots (بنانشانی گرہ_idx, hb_سیٹ_t& لبھیا)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_سیٹ_t ویکھیا;
    أرجع traverse_directed_bfs (گرہ_idx, [&] (
      بنانشانی ماپا,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشانی بچہ,
      بنانشانی _) {

      إذا (link && لبھیا.has(ماپا))
        // Don't traverse from something that's already marked as a root.
        أرجع خطأ;

      إذا (link && !link->is_signed && link->چوڑائی == 4)
      {
        لبھیا.جوڑو (link->objidx);
        ویکھیا.جوڑو (link->objidx);
        أرجع خطأ;
      }

      إذا (ویکھیا.has (بچہ)) أرجع خطأ;
      ویکھیا.جوڑو (بچہ);
      أرجع سچ;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  سانچا<قسم_ناں O>
  گراف_نتیجہ_t<بنانشانی> move_بچہ (بنانشانی old_ماپا_idx,
                                       ثابت O* old_وتھ,
                                       بنانشانی new_ماپا_idx,
                                       ثابت O* new_وتھ)
  {
    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    إذا (unlikely (old_ماپا_idx >= vertices_.لمائی ||
                  new_ماپا_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    خودکار& old_v = vertices_[old_ماپا_idx];
    خودکار& new_v = vertices_[new_ماپا_idx];

    TRY_سونپو (بنانشانی بچہ_id, اشاریہ_for_وتھ (old_ماپا_idx,
                                                     old_وتھ));
    إذا (unlikely (بچہ_id >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    TRY(new_v.جوڑو_real_link(O::static_ناپ, بچہ_id, (ثابت حرف*) new_وتھ - (ثابت حرف*) new_v.obj().سر_2));

    خودکار& بچہ = vertices_[بچہ_id];
    TRY(بچہ.جوڑو_ماپا (new_ماپا_idx, خطأ));

    old_v.remove_real_link_unordered (بچہ_id, old_وتھ);
    بچہ.remove_ماپا (old_ماپا_idx);

    أرجع Ok(بچہ_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  سانچا<قسم_ناں O>
  گراف_نتیجہ_t<فراغ> move_children (بنانشانی old_ماپا_idx,
                                      بنانشانی old_pos_شروع,
                                      بنانشانی old_pos_سرا,
                                      بنانشانی new_ماپا_idx,
                                      بنانشانی new_pos_شروع)
  {
    إذا (unlikely (old_ماپا_idx >= vertices_.لمائی ||
                  new_ماپا_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    خودکار& old_v = vertices_[old_ماپا_idx];
    خودکار& new_v = vertices_[new_ماپا_idx];

    hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t> old_links;
    لكل (ثابت خودکار& l : old_v.obj().real_links)
    {
      إذا (l.position < old_pos_شروع || l.position >= old_pos_سرا)
      {
        old_links.push (l);
        استمر;
      }

      بنانشانی array_pos = l.position - old_pos_شروع;

      بنانشانی بچہ_id = l.objidx;
      إذا (unlikely (بچہ_id >= vertices_.لمائی))
        أرجع Err(باہرلا_دا_BOUNDS);

      TRY(new_v.جوڑو_real_link(O::static_ناپ, بچہ_id, new_pos_شروع + array_pos));

      خودکار& بچہ = vertices_[بچہ_id];
      TRY(بچہ.جوڑو_ماپا (new_ماپا_idx, خطأ));
      بچہ.remove_ماپا (old_ماپا_idx);
    }

    TRY (گراف_نتیجہ_t<فراغ>::from (old_links, ونڈ_FAILURE));
    أرجع old_v.سیٹ_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  گراف_نتیجہ_t<فراغ> duplicate_subgraph (بنانشانی گرہ_idx, hb_map_t& اشاریہ_map)
  {
    گراف_نتیجہ_t<فراغ> نتیجہ = Ok();
    TRY (traverse_directed_bfs (گرہ_idx, [&] (
      بنانشانی ماپا,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بنانشانی بچہ,
      بنانشانی _) {
      إذا (نتیجہ.is_err () || اشاریہ_map.has (بچہ)) أرجع خطأ;
      خودکار r = duplicate (بچہ);
      إذا (unlikely (r.is_err ()))
      {
        نتیجہ = Err(r.error ());
        أرجع خطأ;
      }
      اشاریہ_map.سیٹ (بچہ, *r);
      أرجع سچ;
    }));

    أرجع گراف_نتیجہ_t<فراغ>::from(اشاریہ_map, ونڈ_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  گراف_نتیجہ_t<بنانشانی> duplicate (بنانشانی گرہ_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (گرہ_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    إذا (unlikely (vertices_.لمائی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.لمائی);
      أرجع Err(حد_بندی_EXCEEDED);
    }

    positions_invalid = سچ;
    فاصلہ_invalid = سچ;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجہ_t<فراغ>::from (vertices_, ونڈ_FAILURE));

    بنانشانی clone_idx = vertices_.لمائی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجہ_t<فراغ>::from (ordering_, ونڈ_FAILURE));

    خودکار& بچہ = vertices_[گرہ_idx];

    بنانشانی جدول_ناپ = بچہ.obj().پوچھ - بچہ.obj().سر_2;
    إذا (copy_جدول && جدول_ناپ)
    {
      حرف* بفر = (حرف*) hb_malloc (جدول_ناپ);
      إذا (unlikely (!بفر))
        أرجع Err(ونڈ_FAILURE);
      خودکار res = جوڑو_بفر (بفر);
      إذا (unlikely (res.is_err ()))
      {
        hb_free (بفر);
        أرجع Err(res.error ());
      }
      hb_memcpy (بفر, بچہ.obj().سر_2, جدول_ناپ);
      clone->سیٹ_بفر(بفر, بفر + جدول_ناپ);
    }
    وإلا
    {
      clone->سیٹ_بفر(بچہ.obj().سر_2, بچہ.obj().پوچھ);
    }
    clone->distance = بچہ.distance;
    clone->space = بچہ.space;
    clone->reset_parents ();

    لكل (ثابت خودکار& l : بچہ.obj().real_links)
    {
      TRY(clone->جوڑو_real_link (l));
      TRY(vertices_[l.objidx].جوڑو_ماپا (clone_idx, خطأ));
    }

    لكل (ثابت خودکار& l : بچہ.obj().virtual_links)
    {
      TRY(clone->جوڑو_virtual_link (l));
      TRY(vertices_[l.objidx].جوڑو_ماپا (clone_idx, سچ));
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
  گراف_نتیجہ_t<بنانشانی> duplicate (بنانشانی ماپا_idx, بنانشانی بچہ_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (ماپا_idx >= vertices_.لمائی || بچہ_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& بچہ = vertices_[بچہ_idx];
    بنانشانی links_منزل_بچہ = بچہ.incoming_کنارے_from_ماپا (ماپا_idx);

    إذا (unlikely (بچہ.incoming_کنارے_2 () <= links_منزل_بچہ || بچہ.has_incoming_virtual_کنارے_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  Not duplicating %u => %u",
                 ماپا_idx, بچہ_idx);
      أرجع Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  Duplicating %u => %u",
               ماپا_idx, بچہ_idx);

    TRY_سونپو (بنانشانی clone_idx, duplicate (بچہ_idx, copy_جدول));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    إذا (ماپا_idx == clone_idx) ماپا_idx++;

    خودکار& ماپا = vertices_[ماپا_idx];
    بنانشانی گنتی = 0;
    بنانشانی عدد_real = ماپا.obj().real_links.لمائی;
    لكل (خودکار& l : ماپا.all_links_writer ())
    {
      گنتی++;
      إذا (l.objidx != بچہ_idx)
        استمر;

      TRY(reassign_link (l, ماپا_idx, clone_idx, گنتی > عدد_real));
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
  گراف_نتیجہ_t<بنانشانی> duplicate (ثابت hb_سیٹ_t* parents, بنانشانی بچہ_idx)
  {
    إذا (unlikely (!parents || parents->is_empty ()))
      أرجع Err(INVALID_دلیل);

    إذا (unlikely (بچہ_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& بچہ = vertices_[بچہ_idx];
    بنانشانی links_منزل_بچہ = 0;
    بنانشانی آخری_ماپا = parents->get_max ();
    بنانشانی پہلا_ماپا = parents->get_min ();
    لكل (بنانشانی ماپا_idx : *parents) {
      إذا (unlikely (ماپا_idx >= vertices_.لمائی))
        أرجع Err(باہرلا_دا_BOUNDS);
      links_منزل_بچہ += بچہ.incoming_کنارے_from_ماپا (ماپا_idx);
    }

    إذا (unlikely (بچہ.incoming_کنارے_2 () <= links_منزل_بچہ || بچہ.has_incoming_virtual_کنارے_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  Not duplicating %u, ..., %u => %u", پہلا_ماپا, آخری_ماپا, بچہ_idx);
      أرجع Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  Duplicating %u, ..., %u => %u", پہلا_ماپا, آخری_ماپا, بچہ_idx);

    TRY_سونپو (بنانشانی clone_idx, duplicate (بچہ_idx));

    لكل (بنانشانی ماپا_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      إذا (ماپا_idx == clone_idx) ماپا_idx++;
      خودکار& ماپا = vertices_[ماپا_idx];
      بنانشانی گنتی = 0;
      بنانشانی عدد_real = ماپا.obj().real_links.لمائی;
      لكل (خودکار& l : ماپا.all_links_writer ())
      {
        گنتی++;
        إذا (l.objidx != بچہ_idx)
          استمر;

        TRY(reassign_link (l, ماپا_idx, clone_idx, گنتی > عدد_real));
      }
    }

    أرجع Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  گراف_نتیجہ_t<بنانشانی> new_گرہ (حرف* سر_2, حرف* پوچھ)
  {
    إذا (unlikely (vertices_.لمائی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.لمائی);
      أرجع Err(حد_بندی_EXCEEDED);
    }

    positions_invalid = سچ;
    فاصلہ_invalid = سچ;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجہ_t<فراغ>::from (vertices_, ونڈ_FAILURE));

    بنانشانی clone_idx = vertices_.لمائی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجہ_t<فراغ>::from (ordering_, ونڈ_FAILURE));

    clone->سیٹ_بفر (سر_2, پوچھ);
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
  گراف_نتیجہ_t<بنانشانی> remap_بچہ (بنانشانی ماپا_idx, بنانشانی old_بچہ_idx)
  {
    إذا (unlikely (ماپا_idx >= vertices_.لمائی || old_بچہ_idx >= vertices_.لمائی))
      أرجع Err(باہرلا_دا_BOUNDS);

    TRY_سونپو (بنانشانی new_بچہ_idx, duplicate (old_بچہ_idx));

    خودکار& ماپا = vertices_[ماپا_idx];
    لكل (خودکار& l : ماپا.real_links_writer ())
    {
      إذا (l.objidx != old_بچہ_idx)
        استمر;
      TRY(reassign_link (l, ماپا_idx, new_بچہ_idx, خطأ));
    }

    لكل (خودکار& l : ماپا.virtual_links_writer ())
    {
      إذا (l.objidx != old_بچہ_idx)
        استمر;
      TRY(reassign_link (l, ماپا_idx, new_بچہ_idx, سچ));
    }
    أرجع Ok(new_بچہ_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  گراف_نتیجہ_t<منطقي> raise_childrens_ترجیح (بنانشانی ماپا_idx)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  Raising priority of all children of %u",
               ماپا_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    إذا (unlikely (ماپا_idx >= vertices_.لمائی)) أرجع Err(باہرلا_دا_BOUNDS);
    منطقي made_change = خطأ;
    لكل (خودکار& l : vertices_[ماپا_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_ترجیح ();
    أرجع Ok(made_change);
  }

  گراف_نتیجہ_t<فراغ> is_fully_connected ()
  {
    TRY(update_parents());

    إذا (unlikely (جڑ().incoming_کنارے_2 ()))
      // Root cannot have parents.
      أرجع Err(ORPHANED_NODES);

    لكل (بنانشانی i = 0; i < جڑ_idx (); i++)
    {
      إذا (unlikely (!vertices_[i].incoming_کنارے_2 ()))
        أرجع Err(ORPHANED_NODES);
    }
    أرجع Ok();
  }

#إذا 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  فراغ save_fuzzer_بیج (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((فراغ*) &ٹیگ_2, حجم (ٹیگ_2), 1, f);

    uint16_t عدد_objects = vertices_.لمائی;
    fwrite ((فراغ*) &عدد_objects, حجم (عدد_objects), 1, f);

    لكل (ثابت خودکار& v : vertices_)
    {
      uint16_t blob_ناپ = v.جدول_ناپ ();
      fwrite ((فراغ*) &blob_ناپ, حجم (blob_ناپ), 1, f);
      fwrite ((ثابت فراغ*) v.obj.سر_2, blob_ناپ, 1, f);
    }

    uint16_t link_گنتی = 0;
    لكل (ثابت خودکار& v : vertices_)
      link_گنتی += v.obj.real_links.لمائی;

    fwrite ((فراغ*) &link_گنتی, حجم (link_گنتی), 1, f);

    تعريفنوع بنية
    {
      uint16_t ماپا;
      uint16_t بچہ;
      uint16_t position;
      uint8_t چوڑائی;
    } link_t;

    لكل (بنانشانی i = 0; i < vertices_.لمائی; i++)
    {
      لكل (ثابت خودکار& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.چوڑائی
        };
        fwrite ((فراغ*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#جے_انت

  بنانشانی عدد_roots_for_space_2 (بنانشانی space) ثابت
  {
    أرجع عدد_roots_for_space[space];
  }

  بنانشانی اگلا_space () ثابت
  {
    أرجع عدد_roots_for_space.لمائی;
  }

  گراف_نتیجہ_t<فراغ> move_منزل_new_space (ثابت hb_سیٹ_t& indices)
  {
    إذا (unlikely (عدد_roots_for_space.لمائی >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 عدد_roots_for_space.لمائی);
      أرجع Err(حد_بندی_EXCEEDED);
    }
    عدد_roots_for_space.push (0);
    TRY (گراف_نتیجہ_t<فراغ>::from (عدد_roots_for_space, ونڈ_FAILURE));
    بنانشانی new_space = عدد_roots_for_space.لمائی - 1;

    لكل (بنانشانی اشاریہ_2 : indices) {
      إذا (unlikely (اشاریہ_2 >= vertices_.لمائی))
        أرجع Err(باہرلا_دا_BOUNDS);
      خودکار& node = vertices_[اشاریہ_2];
      عدد_roots_for_space[node.space] = عدد_roots_for_space[node.space] - 1;
      عدد_roots_for_space[new_space] = عدد_roots_for_space[new_space] + 1;
      node.space = new_space;
      فاصلہ_invalid = سچ;
      positions_invalid = سچ;
    }
    أرجع Ok();
  }

  بنانشانی space_for (بنانشانی اشاریہ_2, بنانشانی* جڑ = خالی_سوچک) ثابت
  {
    طالما (سچ)
    {
      أكد (اشاریہ_2 < vertices_.لمائی);
      ثابت خودکار& node = vertices_[اشاریہ_2];
      إذا (node.space)
      {
        إذا (جڑ != خالی_سوچک)
          *جڑ = اشاریہ_2;
        أرجع node.space;
      }

      إذا (!node.incoming_کنارے_2 ())
      {
        إذا (جڑ)
          *جڑ = اشاریہ_2;
        أرجع 0;
      }

      اشاریہ_2 = *node.parents_iter ();
    }
  }

  size_t کل_ناپ_وچ_بائٹاں () ثابت {
    size_t کل_ناپ = 0;
    بنانشانی گنتی = vertices_.لمائی;
    لكل (بنانشانی i = 0; i < گنتی; i++) {
      size_t ناپ = vertices_.arrayZ[i].جدول_ناپ ();
      کل_ناپ += ناپ;
    }
    أرجع کل_ناپ;
  }


 خاص:

  فراغ print_orphaned_nodes ()
  {
    إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Graph is not fully connected.");

    parents_invalid = سچ;
    (فراغ) update_parents();

    إذا (جڑ().incoming_کنارے_2 ()) {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Root node has incoming edges.");
    }

    لكل (بنانشانی i = 0; i < vertices_.لمائی; i++)
    {
      ثابت خودکار& v = vertices_[i];
      إذا (!v.incoming_کنارے_2 ())
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  بنانشانی wide_parents (بنانشانی گرہ_idx, hb_سیٹ_t& parents) ثابت
  {
    بنانشانی گنتی = 0;
    لكل (بنانشانی p : vertices_[گرہ_idx].parents_iter ())
    {
      // Only real links can be wide
      لكل (ثابت خودکار& l : vertices_[p].obj().real_links)
      {
        إذا (l.objidx == گرہ_idx
            && (l.چوڑائی == 3 || l.چوڑائی == 4)
            && !l.is_signed)
        {
          گنتی++;
          parents.جوڑو (p);
        }
      }
    }
    أرجع گنتی;
  }

 عام:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  گراف_نتیجہ_t<فراغ> update_parents ()
  {
    إذا (!parents_invalid) أرجع Ok();

    بنانشانی گنتی = vertices_.لمائی;

    لكل (بنانشانی i = 0; i < گنتی; i++)
      vertices_.arrayZ[i].reset_parents ();

    لكل (بنانشانی p = 0; p < گنتی; p++)
    {
      لكل (ثابت خودکار& l : vertices_.arrayZ[p].obj().real_links)
      {
        إذا (unlikely (l.objidx >= گنتی)) أرجع Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جوڑو_ماپا (p, خطأ));
      }

      لكل (ثابت خودکار& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        إذا (unlikely (l.objidx >= گنتی)) أرجع Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جوڑو_ماپا (p, سچ));
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

    بنانشانی current_pos = 0;
    لكل (بنانشانی i : ordering_)
    {
      خودکار& v = vertices_[i];
      v.شروع = current_pos;
      current_pos += v.obj().پوچھ - v.obj().سر_2;
      v.سرا = current_pos;
    }

    positions_invalid = خطأ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  گراف_نتیجہ_t<فراغ> update_distances ()
  {
    إذا (!فاصلہ_invalid) أرجع Ok();

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
    بنانشانی گنتی = vertices_.لمائی;
    لكل (بنانشانی i = 0; i < گنتی; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[جڑ_idx ()].distance = 0;

    hb_ترجیح_قطار_t<int64_t> قطار;
    قطار.alloc (گنتی);
    قطار.insert (0, جڑ_idx ());
    TRY (گراف_نتیجہ_t<فراغ>::from (قطار, ونڈ_FAILURE));

    hb_سمتیہ_t<منطقي> ویکھیا;
    ویکھیا.resize (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (ویکھیا, ونڈ_FAILURE));

    طالما (!قطار.وچ_error () && !قطار.is_empty ())
    {
      بنانشانی اگلا_idx = قطار.نکالو_minimum ().second;
      إذا (ویکھیا[اگلا_idx]) استمر;
      ثابت خودکار& next = vertices_[اگلا_idx];
      int64_t اگلا_فاصلہ = next.distance;
      ویکھیا[اگلا_idx] = سچ;

      لكل (ثابت خودکار& link : next.obj().all_links ())
      {
        إذا (unlikely (link.objidx >= vertices_.لمائی))
          أرجع Err(باہرلا_دا_BOUNDS);

        إذا (ویکھیا[link.objidx]) استمر;

        خودکار& بچہ_v = vertices_.arrayZ[link.objidx];
        ثابت خودکار& بچہ = بچہ_v.obj ();
        بنانشانی link_چوڑائی = link.چوڑائی ? link.چوڑائی : 4; // treat virtual offsets as 32 bits wide
        int64_t بچہ_بھار = (بچہ.پوچھ - بچہ.سر_2) +
                               ((int64_t) 1 << (link_چوڑائی * 8)) * (بچہ_v.space + 1);
        int64_t بچہ_فاصلہ = اگلا_فاصلہ + بچہ_بھار;

        إذا (بچہ_فاصلہ < بچہ_v.distance)
        {
          بچہ_v.distance = بچہ_فاصلہ;
          قطار.insert (بچہ_فاصلہ, link.objidx);
        }
      }
    }

    TRY (گراف_نتیجہ_t<فراغ>::from (قطار, ونڈ_FAILURE));
    إذا (unlikely (!قطار.is_empty ()))
    {
      print_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    فاصلہ_invalid = خطأ;
    أرجع Ok();
  }

 خاص:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  گراف_نتیجہ_t<فراغ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      بنانشانی ماپا_idx,
                      بنانشانی new_idx,
                      منطقي is_virtual)
  {
    بنانشانی old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_ماپا (ماپا_idx);
    أرجع vertices_[new_idx].جوڑو_ماپا (ماپا_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  سانچا<قسم_ناں Iterator, hb_requires (hb_is_iterator (Iterator))>
  گراف_نتیجہ_t<فراغ> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقي only_wide = خطأ)
  {
    إذا (!id_map) أرجع Ok();
    لكل (بنانشانی i : subgraph)
    {
      خودکار& راس = vertices_[i];
      بنانشانی عدد_real = راس.obj().real_links.لمائی;
      بنانشانی گنتی = 0;
      لكل (خودکار& link : راس.all_links_writer ())
      {
        گنتی++;
        ثابت uint32_t *v;
        إذا (!id_map.has (link.objidx, &v)) استمر;
        إذا (only_wide && (link.is_signed || (link.چوڑائی != 4 && link.چوڑائی != 3))) استمر;

        TRY(reassign_link (link, i, *v, گنتی > عدد_real));
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
  گراف_نتیجہ_t<فراغ> لبھو_connected_nodes (بنانشانی شروع_idx,
                                             hb_سیٹ_t& targets,
                                             hb_سیٹ_t& ویکھیا,
                                             hb_سیٹ_t& connected)
  {
    TRY (گراف_نتیجہ_t<فراغ>::from (ویکھیا, ونڈ_FAILURE));
    إذا (ویکھیا.has (شروع_idx)) أرجع Ok();
    إذا (unlikely (شروع_idx >= vertices_.لمائی))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      أرجع Err(باہرلا_دا_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.لمائی);
    TRY (گراف_نتیجہ_t<فراغ>::from (ordering_scratch_, ونڈ_FAILURE));

    خودکار& ڈھیر = ordering_scratch_;
    بنانشانی ڈھیر_len = 0;

    گراف_نتیجہ_t<فراغ> res = Ok();
    خودکار handle_گرہ = [&] (بنانشانی گرہ_idx) {
      ویکھیا.جوڑو (گرہ_idx);
      إذا (targets.has (گرہ_idx)) {
        targets.del (گرہ_idx);
        connected.جوڑو (گرہ_idx);
      }

      إذا (unlikely (ڈھیر_len >= ڈھیر.لمائی))
      {
        res = Err(چکر_DETECTED);
        أرجع خطأ;
      }
      ڈھیر[ڈھیر_len++] = گرہ_idx;
      أرجع سچ;
    };

    إذا (!handle_گرہ (شروع_idx)) أرجع res;

    طالما (ڈھیر_len > 0)
    {
      بنانشانی گرہ_idx = ڈھیر[--ڈھیر_len];
      ثابت خودکار& v = vertices_[گرہ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      لكل (ثابت خودکار& l : v.obj().all_links ())
      {
        بنانشانی بچہ_idx = l.objidx;
        إذا (ویکھیا.has (بچہ_idx)) استمر;
        إذا (!handle_گرہ (بچہ_idx)) أرجع res;
      }

      لكل (بنانشانی ماپا_idx : v.parents_iter ())
      {
        إذا (ویکھیا.has (ماپا_idx)) استمر;
        إذا (!handle_گرہ (ماپا_idx)) أرجع res;
      }
    }

    TRY (گراف_نتیجہ_t<فراغ>::from (ویکھیا, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (connected, ونڈ_FAILURE));

    أرجع Ok();
  }

 عام:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_سمتیہ_t<راس_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_سمتیہ_t<بنانشانی> ordering_;
  hb_سمتیہ_t<بنانشانی> ordering_scratch_;

 خاص:
  منطقي parents_invalid = سچ;
  منطقي فاصلہ_invalid = سچ;
  منطقي positions_invalid = سچ;
  hb_سمتیہ_t<بنانشانی> عدد_roots_for_space;
  hb_سمتیہ_t<حرف*> buffers;
};

}

#جے_انت  // GRAPH_GRAPH_HH
