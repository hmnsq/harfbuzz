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

#که_نه_تعریف_شوی ګراف_ګراف_HH
#تعریف_کړه ګراف_ګراف_HH

#تعریف_کړه HB_ګراف_INVALID ((بینښه) -1)

نطاق ګراف {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
بنية ګراف_t
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
    بینښه space = 0 ;
    بینښه پیل = 0;
    بینښه پای_ټکی = 0;
    بینښه لومړیتوب = 0;

    خاص:
    hb_serialize_context_t::object_t obj_;
    بینښه incoming_څنډې = 0;
    بینښه single_پلار = HB_ګراف_INVALID;
    منطقي has_incoming_virtual_څنډې = خطأ;
    hb_hashmap_t<بینښه, بینښه> parents;
    hb_سېټ_t virtual_parents;
    عام:

    راس_t ()
    {}

    څرګند راس_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    خپلکاری parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_پلار, single_پلار != HB_ګراف_INVALID),
	parents.کلیګانې_ref ()
      )
    )

    فراغ سېټ_بفر (حرف* سر_2, حرف* لکۍ) {
      obj_.سر_2 = سر_2;
      obj_.لکۍ = لکۍ;
    }

    فراغ shrink_بفر (size_t شمېر)
    {
      obj_.لکۍ -= شمېر;
    }

    خپلکاری all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    خپلکاری real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    خپلکاری virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    فراغ ترتیبول_real_links () {
      obj_.real_links.qsort ();
    }

    ګراف_پایله_t<فراغ> جمع_کړه_real_link (بینښه پلنوالی, بینښه obj_idx, بینښه position) {
      خپلکاری* link = obj_.real_links.push ();
      TRY (ګراف_پایله_t<فراغ>::from (obj_.real_links, ویش_FAILURE));
      link->پلنوالی = پلنوالی;
      link->objidx = obj_idx;
      link->position = position;
      أرجع Ok();
    }

    ګراف_پایله_t<فراغ> جمع_کړه_virtual_link (بینښه obj_idx) {
      خپلکاری* link = obj_.virtual_links.push ();
      TRY (ګراف_پایله_t<فراغ>::from (obj_.virtual_links, ویش_FAILURE));
      link->objidx = obj_idx;
      أرجع Ok();
    }

    ګراف_پایله_t<فراغ> جمع_کړه_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      أرجع ګراف_پایله_t<فراغ>::from (obj_.real_links, ویش_FAILURE);
    }

    ګراف_پایله_t<فراغ> سېټ_real_links (hb_ویکټور_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(ګراف_پایله_t<فراغ>::from (links, ویش_FAILURE));
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

    ګراف_پایله_t<فراغ> جمع_کړه_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      أرجع ګراف_پایله_t<فراغ>::from (obj_.virtual_links, ویش_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { أرجع obj_; }

    کينډۍ<ډول_نوم T>
    T as () ثابت
    {
      أرجع reinterpret_cast<T> (obj_.سر_2);
    }

    منطقي has_incoming_virtual_څنډې_2 () ثابت
    {
      أرجع has_incoming_virtual_څنډې;
    }

    ګراف_پایله_t<فراغ> link_positions_valid (بینښه عدد_objects, منطقي removed_nil)
    {
      hb_سېټ_t assigned_بایټونه;
      لكل (ثابت خپلکاری& l : obj_.real_links)
      {
        إذا (unlikely (l.objidx >= عدد_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                     "Invalid graph. Invalid object index.");
          أرجع Err(INVALID_دلیل);
        }

        بینښه پیل = l.position;
        بینښه پای_ټکی = پیل + l.پلنوالی - 1;

        إذا (unlikely (l.پلنوالی < 2 || l.پلنوالی > 4))
        {
          DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                     "Invalid graph. Invalid link width.");
          أرجع Err(INVALID_دلیل);
        }

        إذا (unlikely (پای_ټکی >= جدول_اندازه ()))
        {
          DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                     "Invalid graph. Link position is out of bounds.");
          أرجع Err(بهر_د_BOUNDS);
        }

        إذا (unlikely (assigned_بایټونه.intersects (پیل, پای_ټکی)))
        {
          DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                     "Invalid graph. Found offsets whose positions overlap.");
          أرجع Err(INVALID_دلیل);
        }

        assigned_بایټونه.جمع_کړه_range (پیل, پای_ټکی);
      }

      أرجع ګراف_پایله_t<فراغ>::from (assigned_بایټونه, ویش_FAILURE);
    }

    فراغ normalize ()
    {
      obj_.real_links.qsort ();
      لكل (ثابت خپلکاری& l : obj_.real_links)
      {
        لكل (بینښه i = 0; i < l.پلنوالی; i++)
        {
          obj_.سر_2[l.position + i] = 0;
        }
      }
    }

    منطقي equals (بینښه this_شاخص,
                 بینښه other_شاخص,
                 ثابت راس_t& other,
                 ثابت ګراف_t& ګراف,
                 ثابت ګراف_t& other_ګراف,
                 بینښه ژوروالی) ثابت
    {
      إذا (!(as_بایټونه () == other.as_بایټونه ()))
      {
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_شاخص,
                   (بینښه اوږد) جدول_اندازه (),
                   other_شاخص,
                   (بینښه اوږد) other.جدول_اندازه (),
                   ژوروالی);

        خپلکاری a = as_بایټونه ();
        خپلکاری b = other.as_بایټونه ();
        طالما (a || b)
        {
          DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                     "  0x%x %s 0x%x", (بینښه) *a, (*a == *b) ? "==" : "!=", (بینښه) *b);
          a++;
          b++;
        }
        أرجع خطأ;
      }

      أرجع links_equal (obj_.real_links, other.obj_.real_links, ګراف, other_ګراف, ژوروالی);
    }

    hb_بایټونه_t as_بایټونه () ثابت
    {
      أرجع hb_بایټونه_t (obj_.سر_2, جدول_اندازه ());
    }

    ملګری فراغ swap (راس_t& a, راس_t& b)
    {
      hb_بدلون (a.obj_, b.obj_);
      hb_بدلون (a.distance, b.distance);
      hb_بدلون (a.space, b.space);
      hb_بدلون (a.single_پلار, b.single_پلار);
      hb_بدلون (a.parents, b.parents);
      hb_بدلون (a.virtual_parents, b.virtual_parents);
      hb_بدلون (a.incoming_څنډې, b.incoming_څنډې);
      hb_بدلون (a.has_incoming_virtual_څنډې, b.has_incoming_virtual_څنډې);
      hb_بدلون (a.پیل, b.پیل);
      hb_بدلون (a.پای_ټکی, b.پای_ټکی);
      hb_بدلون (a.لومړیتوب, b.لومړیتوب);
    }

    ګراف_پایله_t<hb_hashmap_t<بینښه, بینښه>>
    position_منزل_شاخص_map () ثابت
    {
      hb_hashmap_t<بینښه, بینښه> پایله;

      پایله.alloc (obj_.real_links.اوږدتیا);
      لكل (ثابت خپلکاری& l : obj_.real_links) {
        پایله.سېټ (l.position, l.objidx);
      }

      أرجع ګراف_پایله_t<hb_hashmap_t<بینښه, بینښه>>::from (std::move(پایله), ویش_FAILURE);
    }

    منطقي is_shared () ثابت
    {
      أرجع parents.get_population () > virtual_parents.get_population () + 1;
    }

    بینښه incoming_څنډې_2 () ثابت
    {
      إذا (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_څنډې == (single_پلار != HB_ګراف_INVALID) +
		(parents.ارزښتونه_ref () | hb_reduce (hb_جمع_کړه, 0)));
       }
      أرجع incoming_څنډې;
    }

    بینښه incoming_څنډې_from_پلار (بینښه پلار_شاخص) ثابت {
      إذا (single_پلار != HB_ګراف_INVALID) {
        أرجع single_پلار == پلار_شاخص ? 1 : 0;
      }

      بینښه* شمېر;
      أرجع  parents.has(پلار_شاخص, &شمېر) ? *شمېر : 0;
    }

    فراغ reset_parents ()
    {
      incoming_څنډې = 0;
      has_incoming_virtual_څنډې = خطأ;
      single_پلار = HB_ګراف_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    ګراف_پایله_t<فراغ> جمع_کړه_پلار (بینښه پلار_شاخص, منطقي is_virtual)
    {
      أكد (پلار_شاخص != HB_ګراف_INVALID);
      has_incoming_virtual_څنډې |= is_virtual;
      إذا (is_virtual)
        virtual_parents.جمع_کړه (پلار_شاخص);

      إذا (incoming_څنډې == 0)
      {
	single_پلار = پلار_شاخص;
	incoming_څنډې = 1;
	أرجع منزل_پایله();
      }
      وإلا إذا (single_پلار != HB_ګراف_INVALID)
      {
        أكد (incoming_څنډې == 1);
	إذا (!parents.سېټ (single_پلار, 1))
	  أرجع منزل_پایله ();
	single_پلار = HB_ګراف_INVALID;
      }

      بینښه *v;
      إذا (parents.has (پلار_شاخص, &v))
      {
        (*v)++;
	incoming_څنډې++;
      }
      وإلا إذا (parents.سېټ (پلار_شاخص, 1))
	incoming_څنډې++;

      أرجع منزل_پایله();
    }

    فراغ remove_پلار (بینښه پلار_شاخص)
    {
      إذا (پلار_شاخص == single_پلار)
      {
	single_پلار = HB_ګراف_INVALID;
	incoming_څنډې--;
	virtual_parents.reset ();
	أرجع;
      }

      بینښه *v;
      إذا (parents.has (پلار_شاخص, &v))
      {
	incoming_څنډې--;
	إذا (*v > 1)
	  (*v)--;
	وإلا
        {
	  parents.del (پلار_شاخص);
          virtual_parents.del (پلار_شاخص);
        }

	إذا (incoming_څنډې == 1)
	{
	  single_پلار = *parents.کلیګانې ();
	  parents.reset ();
	}
      }
    }

    فراغ remove_real_link_unordered (بینښه link_array_شاخص) {
      obj_.real_links.remove_unordered (link_array_شاخص);
    }

    فراغ remove_real_link_unordered (بینښه ماشوم_شاخص, ثابت فراغ* دننه_واټن)
    {
      بینښه شمېر = obj_.real_links.اوږدتیا;
      لكل (بینښه i = 0; i < شمېر; i++)
      {
        خپلکاری& link = obj_.real_links.arrayZ[i];
        إذا (link.objidx != ماشوم_شاخص)
          استمر;

        إذا ((obj_.سر_2 + link.position) != دننه_واټن)
          استمر;

        obj_.real_links.remove_unordered (i);
        أرجع;
      }
    }

    ګراف_پایله_t<فراغ> remap_پلار (بینښه old_شاخص, بینښه new_شاخص)
    {
      إذا (single_پلار != HB_ګراف_INVALID)
      {
        إذا (single_پلار == old_شاخص)
        {
	  single_پلار = new_شاخص;
          إذا (virtual_parents.has (old_شاخص))
          {
            virtual_parents.del (old_شاخص);
            virtual_parents.جمع_کړه (new_شاخص);
          }
        }
        أرجع ګراف_پایله_t<فراغ>::from(virtual_parents, ویش_FAILURE);
      }

      ثابت بینښه *pv;
      إذا (parents.has (old_شاخص, &pv))
      {
        بینښه v = *pv;
	إذا (!parents.سېټ (new_شاخص, v))
          incoming_څنډې -= v;
	parents.del (old_شاخص);

        إذا (incoming_څنډې == 1)
	{
	  single_پلار = *parents.کلیګانې ();
	  parents.reset ();
	}
      }

      إذا (virtual_parents.has (old_شاخص))
      {
        virtual_parents.del (old_شاخص);
        virtual_parents.جمع_کړه (new_شاخص);
      }

      أرجع ګراف_پایله_t<فراغ>::from(virtual_parents, ویش_FAILURE);
    }

    منطقي is_ونې_پاڼه () ثابت
    {
      أرجع !obj_.real_links.اوږدتیا && !obj_.virtual_links.اوږدتیا;
    }

    منطقي raise_لومړیتوب ()
    {
      إذا (has_max_لومړیتوب ()) أرجع خطأ;
      لومړیتوب++;
      أرجع ریښتیا;
    }

    منطقي give_max_لومړیتوب ()
    {
      منطقي پایله = خطأ;
      طالما (!has_max_لومړیتوب()) {
        پایله = ریښتیا;
        لومړیتوب++;
      }
      أرجع پایله;
    }

    منطقي has_max_لومړیتوب () ثابت {
      أرجع لومړیتوب >= 3;
    }

    size_t جدول_اندازه () ثابت {
      أرجع obj_.لکۍ - obj_.سر_2;
    }

    int64_t modified_واټن (بینښه ترتیب) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_واټن =
          hb_clamp (distance + واټن_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      إذا (has_max_لومړیتوب ()) {
        modified_واټن = 0;
      }
      أرجع (modified_واټن << 18) | (0x003FFFF & ترتیب);
    }

    int64_t واټن_modifier () ثابت
    {
      إذا (!لومړیتوب) أرجع 0;
      int64_t جدول_اندازه = obj_.لکۍ - obj_.سر_2;

      إذا (لومړیتوب == 1)
        أرجع -جدول_اندازه / 2;

      أرجع -جدول_اندازه;
    }

   خاص:
    ګراف_پایله_t<فراغ> منزل_پایله() ثابت {
      إذا (unlikely (
        parents.دننه_error() || virtual_parents.دننه_error() ||
        obj_.real_links.دننه_error() || obj_.virtual_links.دننه_error()))
        أرجع Err(ویش_FAILURE);
      أرجع Ok();
    }

    منطقي links_equal (ثابت hb_ویکټور_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_ویکټور_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت ګراف_t& ګراف,
                      ثابت ګراف_t& other_ګراف,
                      بینښه ژوروالی) ثابت
    {
      خپلکاری a = this_links.iter ();
      خپلکاری b = other_links.iter ();

      طالما (a && b)
      {
        ثابت خپلکاری& link_a = *a;
        ثابت خپلکاری& link_b = *b;

        إذا (link_a.پلنوالی != link_b.پلنوالی ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          أرجع خطأ;

        إذا (!ګراف.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_ګراف.vertices_[link_b.objidx], ګراف, other_ګراف, ژوروالی + 1))
          أرجع خطأ;

        a++;
        b++;
      }

      إذا (منطقي (a) != منطقي (b))
        أرجع خطأ;

      أرجع ریښتیا;
    }
  };

  تعداد راس_mutability_t
  {
    Immutable,
    Mutable
  };

  کينډۍ <ډول_نوم T, راس_mutability_t mutability = Mutable>
  بنية راس_and_جدول_t
  {
    راس_and_جدول_t () : شاخص_2 (0), راس (تش_اشاره), جدول (تش_اشاره)
    {}

    کينډۍ <راس_mutability_t other_mutability>
    راس_and_جدول_t (ثابت راس_and_جدول_t<T, other_mutability>& o)
      : شاخص_2 (o.شاخص_2), راس (o.راس), جدول (o.جدول)
    {}

    بینښه شاخص_2;
    ډول_نوم std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type راس;
    ډول_نوم std::conditional<mutability == Immutable, ثابت T*, T*>::type جدول;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  کينډۍ<ډول_نوم T>
  ساكن ګراف_پایله_t<ګراف_t> create (ثابت T& objects)
  {
    إذا (unlikely (objects.اوږدتیا > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.اوږدتیا);
      أرجع Err(حد_EXCEEDED);
    }

    ګراف_t g;
    g.عدد_roots_for_space.push (1);
    TRY (ګراف_پایله_t<فراغ>::from (g.عدد_roots_for_space, ویش_FAILURE));

    منطقي removed_nil = خطأ;
    ثابت بینښه شمېر = objects.اوږدتیا;
    g.vertices_.alloc (شمېر);
    g.ordering_.resize (شمېر);
    g.ordering_scratch_.alloc (شمېر);
    TRY (ګراف_پایله_t<فراغ>::from (g.vertices_, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (g.ordering_, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (g.ordering_scratch_, ویش_FAILURE));

    بینښه ترتیب = objects.اوږدتیا;
    بینښه skip = 0;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      إذا (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = ریښتیا;
        ترتیب--;
        g.ordering_.resize (شمېر - 1);
        skip++;
        استمر;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      إذا (unlikely (obj.دننه_error ())) {
        أرجع Err(ویش_FAILURE);
      }
      راس_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (شمېر, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      بینښه obj_idx = i - skip;
      g.ordering_[--ترتیب] = obj_idx;

      إذا (!removed_nil) استمر;
      // Fix indices to account for removed nil object.
      لكل (خپلکاری& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    خپلکاری r = g.is_fully_connected();
    إذا (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    أرجع Ok (std::move (g));
  }

  ګراف_t (ثابت ګراف_t&) = ړنګول;
  ګراف_t& چلوونکی = (ثابت ګراف_t&) = ړنګول;
  ګراف_t (ګراف_t&&) = افتراضي;
  ګراف_t& چلوونکی = (ګراف_t&&) = افتراضي;

  ~ګراف_t ()
  {
    لكل (حرف* b : buffers)
      hb_free (b);
  }

خاص:
  ګراف_t () = افتراضي;

عام:

  منطقي چلوونکی== (ثابت ګراف_t& other) ثابت
  {
    أرجع ریښه ().equals (ریښه_idx(), other.ریښه_idx(), other.ریښه (), *هذا, other, 0);
  }

  فراغ print () ثابت {
    لكل (بینښه id : ordering_)
    {
      ثابت خپلکاری& v = vertices_[id];
      چاپ_کړه("%u: %u [", id, (بینښه صحيح)v.جدول_اندازه());
      لكل (ثابت خپلکاری &l : v.obj().real_links) {
        چاپ_کړه("%u, ", l.objidx);
      }
      لكل (ثابت خپلکاری &l : v.obj().virtual_links) {
        چاپ_کړه("v%u, ", l.objidx);
      }
      چاپ_کړه("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  فراغ normalize ()
  {
    لكل (خپلکاری& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت راس_t& ریښه () ثابت
  {
    أرجع vertices_[ریښه_idx ()];
  }

  بینښه ریښه_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    أرجع ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (بینښه i) ثابت
  {
    أرجع vertices_[i].obj();
  }

  ګراف_پایله_t<فراغ> جمع_کړه_بفر (حرف* بفر)
  {
    buffers.push (بفر);
    أرجع ګراف_پایله_t<فراغ>::from (buffers, ویش_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  کينډۍ<ډول_نوم T>
  ګراف_پایله_t<فراغ> جمع_کړه_link (T* دننه_واټن,
                                 بینښه پلار_id,
                                 بینښه ماشوم_id)
  {
    إذا (unlikely (پلار_id >= vertices_.اوږدتیا || ماشوم_id >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);
    خپلکاری& v = vertices_[پلار_id];
    TRY(v.جمع_کړه_real_link(2, ماشوم_id, (حرف*) دننه_واټن - (حرف*) v.obj().سر_2));
    أرجع vertices_[ماشوم_id].جمع_کړه_پلار (پلار_id, خطأ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  ګراف_پایله_t<فراغ> ترتیبول_تر_ټولو_لنډ_واټن_if_needed ()
  {
    إذا (!positions_invalid) أرجع Ok();
    أرجع ترتیبول_تر_ټولو_لنډ_واټن ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  ګراف_پایله_t<فراغ> ترتیبول_تر_ټولو_لنډ_واټن ()
  {
    positions_invalid = ریښتیا;

    إذا (vertices_.اوږدتیا <= 1) {
      // Graph of 1 or less doesn't need sorting.
      أرجع Ok();
    }

    TRY (update_distances ());

    hb_لومړیتوب_کتار_t<int64_t> کتار;
    کتار.alloc (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (کتار, ویش_FAILURE));

    hb_ویکټور_t<بینښه> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (new_ordering, ویش_FAILURE));

    hb_ویکټور_t<بینښه> removed_څنډې;
    removed_څنډې.resize (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (removed_څنډې, ویش_FAILURE));

    TRY (update_parents ());

    کتار.insert (ریښه ().modified_واټن (0), ریښه_idx ());
    بینښه ترتیب = 1;
    بینښه pos = 0;
    طالما (!کتار.دننه_error () && !کتار.is_empty ())
    {
      بینښه بل_id = کتار.راوباسه_minimum().second;

      إذا (unlikely (pos >= new_ordering.اوږدتیا)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Invalid graph. Contains cycle.");
        أرجع Err(دوره_DETECTED);
      }
      new_ordering[pos++] = بل_id;
      ثابت راس_t& next = vertices_[بل_id];

      لكل (ثابت خپلکاری& link : next.obj().all_links ()) {
        إذا (unlikely (link.objidx >= vertices_.اوږدتیا))
          أرجع Err(بهر_د_BOUNDS);
        removed_څنډې[link.objidx]++;
        ثابت خپلکاری& v = vertices_[link.objidx];
        إذا (!(v.incoming_څنډې_2 () - removed_څنډې[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          کتار.insert (v.modified_واټن (ترتیب++),
                        link.objidx);
      }
    }

    TRY (ګراف_پایله_t<فراغ>::from (کتار, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (new_ordering, ویش_FAILURE));

    hb_بدلون (ordering_, new_ordering);

    إذا (unlikely (pos != vertices_.اوږدتیا)) {
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
  ګراف_پایله_t<فراغ> پیدا_کړه_space_roots (hb_سېټ_t& لیدل_شوی, hb_سېټ_t& roots)
  {
    بینښه ریښه_شاخص = ریښه_idx ();
    لكل (بینښه i : ordering_)
    {
      إذا (لیدل_شوی.has (i)) استمر;

      // Only real links can form 32 bit spaces
      لكل (ثابت خپلکاری& l : vertices_[i].obj().real_links)
      {
        إذا (l.is_signed || l.پلنوالی < 3)
          استمر;

        إذا (i == ریښه_شاخص && l.پلنوالی == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          استمر;

        إذا (l.پلنوالی >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_سېټ_t sub_roots;
          TRY (پیدا_کړه_32bit_roots (l.objidx, sub_roots));
          إذا (sub_roots) {
            لكل (بینښه sub_ریښه_idx : sub_roots) {
              roots.جمع_کړه (sub_ریښه_idx);
              TRY (پیدا_کړه_subgraph (sub_ریښه_idx, لیدل_شوی));
            }
            استمر;
          }
        }

        roots.جمع_کړه (l.objidx);
        TRY (پیدا_کړه_subgraph (l.objidx, لیدل_شوی));
      }
    }

    TRY(ګراف_پایله_t<فراغ>::from (لیدل_شوی, ویش_FAILURE));
    TRY(ګراف_پایله_t<فراغ>::from (roots, ویش_FAILURE));
    أرجع Ok();
  }

  کينډۍ <ډول_نوم T, ډول_نوم ...Ts>
  ګراف_پایله_t<راس_and_جدول_t<T, Immutable>> as_جدول (بینښه پلار, ثابت فراغ* دننه_واټن, Ts... ds)
  {
    TRY_ټاکل (بینښه idx, شاخص_for_دننه_واټن (پلار, دننه_واټن));
    أرجع as_جدول_from_شاخص<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  کينډۍ <ډول_نوم T, ډول_نوم ...Ts>
  ګراف_پایله_t<راس_and_جدول_t<T, Mutable>> as_mutable_جدول (بینښه پلار, ثابت فراغ* دننه_واټن, Ts... ds)
  {
    TRY_ټاکل (خپلکاری idx, mutable_شاخص_for_دننه_واټن (پلار, دننه_واټن));
    أرجع as_جدول_from_شاخص<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  کينډۍ <ډول_نوم T, راس_mutability_t mutability = Mutable, ډول_نوم ...Ts>
  ګراف_پایله_t<راس_and_جدول_t<T, mutability>> as_جدول_from_شاخص (بینښه شاخص_2, Ts... ds)
  {
    إذا (unlikely (شاخص_2 >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    راس_and_جدول_t<T, mutability> r;
    r.راس = (ډول_نوم std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type) &vertices_[شاخص_2];
    r.جدول = r.راس->کينډۍ as<ډول_نوم std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.شاخص_2 = شاخص_2;
    إذا (unlikely (!r.جدول))
      أرجع Err(INVALID_دلیل);

    TRY(r.جدول->sanitize (*(r.راس), std::forward<Ts>(ds)...));
    أرجع r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  ګراف_پایله_t<بینښه> شاخص_for_دننه_واټن (بینښه غوټه_idx, ثابت فراغ* دننه_واټن) ثابت
  {
    إذا (unlikely (غوټه_idx >= vertices_.اوږدتیا)) أرجع Err(بهر_د_BOUNDS);
    ثابت خپلکاری& node = object (غوټه_idx);
    إذا (unlikely (دننه_واټن < node.سر_2 || دننه_واټن >= node.لکۍ)) أرجع Err(بهر_د_BOUNDS);

    بینښه شمېر = node.real_links.اوږدتیا;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت خپلکاری& link = node.real_links.arrayZ[i];
      إذا (دننه_واټن != node.سر_2 + link.position)
        استمر;
      أرجع Ok(link.objidx);
    }

    أرجع Err(INVALID_دلیل);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  ګراف_پایله_t<بینښه> mutable_شاخص_for_دننه_واټن (بینښه غوټه_idx, ثابت فراغ* دننه_واټن)
  {
    TRY_ټاکل (بینښه ماشوم_idx, شاخص_for_دننه_واټن (غوټه_idx, دننه_واټن));
    إذا (unlikely (ماشوم_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    خپلکاری& ماشوم = vertices_[ماشوم_idx];
    لكل (بینښه p : ماشوم.parents_iter ())
    {
      إذا (p != غوټه_idx) {
        أرجع duplicate (غوټه_idx, ماشوم_idx, ریښتیا);
      }
    }

    أرجع Ok(ماشوم_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  ګراف_پایله_t<منطقي> ټاکل_spaces ()
  {
    TRY (update_parents ());

    لكل (خپلکاری& راس : vertices_.writer ())
    {
      راس.space = 0;
      راس.لومړیتوب = 0;
    }
    عدد_roots_for_space.resize (1);
    TRY (ګراف_پایله_t<فراغ>::from (عدد_roots_for_space, ویش_FAILURE));
    عدد_roots_for_space[0] = 1;
    واټن_invalid = ریښتیا;
    positions_invalid = ریښتیا;

    hb_سېټ_t لیدل_شوی;
    hb_سېټ_t roots;
    TRY (پیدا_کړه_space_roots (لیدل_شوی, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    لیدل_شوی.invert ();

    إذا (!roots) أرجع Ok(خطأ);

    طالما (roots)
    {
      uint32_t next = HB_سېټ_ارزښت_INVALID;
      TRY (ګراف_پایله_t<فراغ>::from (roots, ویش_FAILURE));
      إذا (!roots.next (&next)) اكسر;

      hb_سېټ_t connected_roots;
      TRY (پیدا_کړه_connected_nodes (next, roots, لیدل_شوی, connected_roots));
      TRY (ګراف_پایله_t<فراغ>::from (connected_roots, ویش_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (ګراف_پایله_t<فراغ>::from (connected_roots, ویش_FAILURE));

      بینښه بل_space = هذا->بل_space ();
      إذا (unlikely (بل_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 بل_space);
        أرجع Err(حد_EXCEEDED);
      }
      عدد_roots_for_space.push (0);
      TRY(ګراف_پایله_t<فراغ>::from (عدد_roots_for_space, ویش_FAILURE));

      لكل (بینښه ریښه : connected_roots)
      {
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Subgraph %u gets space %u", ریښه, بل_space);
        vertices_[ریښه].space = بل_space;
        عدد_roots_for_space[بل_space] = عدد_roots_for_space[بل_space] + 1;
        واټن_invalid = ریښتیا;
        positions_invalid = ریښتیا;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    أرجع Ok(ریښتیا);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  ګراف_پایله_t<منطقي> isolate_subgraph (hb_سېټ_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_سېټ_t parents;
    لكل (بینښه ریښه_idx : roots)
    {
      إذا (unlikely (ریښه_idx >= vertices_.اوږدتیا))
        أرجع Err(بهر_د_BOUNDS);
      subgraph.سېټ (ریښه_idx, wide_parents (ریښه_idx, parents));
    }
    لكل (بینښه ریښه_idx : roots)
    {
      TRY (پیدا_کړه_subgraph (ریښه_idx, subgraph));
    }
    TRY (ګراف_پایله_t<فراغ>::from (subgraph, ویش_FAILURE));

    hb_map_t شاخص_map;
    منطقي made_changes = خطأ;
    لكل (خپلکاری ثبت : subgraph.iter ())
    {
      أكد (ثبت.لومړی < vertices_.اوږدتیا);
      ثابت خپلکاری& node = vertices_[ثبت.لومړی];
      بینښه subgraph_incoming_څنډې = ثبت.second;

      إذا (subgraph_incoming_څنډې < node.incoming_څنډې_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = ریښتیا;
        TRY (duplicate_subgraph (ثبت.لومړی, شاخص_map));
      }
    }

    TRY (ګراف_پایله_t<فراغ>::from (شاخص_map, ویش_FAILURE));

    إذا (!made_changes)
      أرجع Ok(خطأ);

    خپلکاری new_subgraph =
        + subgraph.کلیګانې ()
        | hb_map([&] (uint32_t غوټه_idx) {
          ثابت uint32_t *v;
          إذا (شاخص_map.has (غوټه_idx, &v)) أرجع *v;
          أرجع غوټه_idx;
        })
        ;

    TRY(remap_obj_indices (شاخص_map, new_subgraph));
    TRY(remap_obj_indices (شاخص_map, parents.iter (), ریښتیا));

    // Update roots set with new indices as needed.
    لكل (خپلکاری next : roots)
    {
      ثابت uint32_t *v;
      إذا (شاخص_map.has (next, &v))
      {
        roots.del (next);
        roots.جمع_کړه (*v);
      }
    }

    أرجع Ok(ریښتیا);
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
  کينډۍ <ډول_نوم لیدنه_څنډه_Func>
  ګراف_پایله_t<فراغ> traverse_directed_bfs (بینښه پیل_idx, لیدنه_څنډه_Func&& لیدنه_څنډه)
  {
    إذا (unlikely (پیل_idx >= vertices_.اوږدتیا))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      أرجع Err(بهر_د_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (ordering_scratch_, ویش_FAILURE));

    إذا (!لیدنه_څنډه (HB_CODEPOINT_INVALID, تش_اشاره, پیل_idx, 0))
      أرجع Ok();

    بینښه سر_2 = 0;
    بینښه لکۍ = 0;
    خپلکاری& کتار = ordering_scratch_;
    کتار[لکۍ++] = پیل_idx;

    بینښه ژوروالی = 0;
    طالما (سر_2 < لکۍ)
    {
      بینښه level_پای_ټکی = لکۍ;
      طالما (سر_2 < level_پای_ټکی)
      {
        بینښه غوټه_idx = کتار[سر_2++];
        ثابت خپلکاری& v = vertices_[غوټه_idx];

        بینښه عدد_real = v.obj().real_links.اوږدتیا;
        بینښه ټول_links = عدد_real + v.obj().virtual_links.اوږدتیا;

        لكل (بینښه i = 0; i < ټول_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت خپلکاری& v = vertices_[غوټه_idx];
          ثابت خپلکاری& link = (i < عدد_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - عدد_real];
          بینښه ماشوم_idx = link.objidx;

          إذا (!لیدنه_څنډه (غوټه_idx, &link, ماشوم_idx, ژوروالی + 1))
            استمر;

          إذا (unlikely (لکۍ >= کتار.اوږدتیا))
            أرجع Err(دوره_DETECTED);

          کتار[لکۍ++] = ماشوم_idx;
        }
      }
      ژوروالی++;
    }
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> پیدا_کړه_subgraph (بینښه غوټه_idx, hb_map_t& subgraph)
  {
    أرجع traverse_directed_bfs (غوټه_idx, [&] (
      بینښه پلار,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بینښه ماشوم,
      بینښه ژوروالی) {
      إذا (ژوروالی == 0) أرجع ریښتیا;
      hb_codepoint_t *شمېر;
      إذا (subgraph.has (ماشوم, &شمېر))
      {
        (*شمېر)++;
        أرجع خطأ;
      }
      subgraph.سېټ (ماشوم, 1);
      أرجع ریښتیا;
    });
  }

  ګراف_پایله_t<فراغ> پیدا_کړه_subgraph (بینښه غوټه_idx, hb_سېټ_t& subgraph)
  {
    أرجع traverse_directed_bfs (غوټه_idx, [&] (
      بینښه پلار,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بینښه ماشوم,
      بینښه ژوروالی) {
      إذا (subgraph.has (ماشوم)) أرجع خطأ;
      subgraph.جمع_کړه (ماشوم);
      أرجع ریښتیا;
    });
  }

  ګراف_پایله_t<size_t> پیدا_کړه_subgraph_اندازه (بینښه غوټه_idx, hb_سېټ_t& subgraph, بینښه max_ژوروالی = HB_ګراف_INVALID)
  {
    size_t اندازه = 0;
    TRY (traverse_directed_bfs (غوټه_idx, [&] (
      بینښه پلار,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بینښه ماشوم,
      بینښه ژوروالی) {
      إذا (subgraph.has (ماشوم)) أرجع خطأ;
      subgraph.جمع_کړه (ماشوم);

      ثابت خپلکاری& o = vertices_[ماشوم].obj();
      اندازه += o.لکۍ - o.سر_2;
      أرجع ژوروالی < max_ژوروالی;
    }));
    أرجع Ok(اندازه);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  ګراف_پایله_t<فراغ> پیدا_کړه_32bit_roots (بینښه غوټه_idx, hb_سېټ_t& وموندل)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_سېټ_t لیدل_شوی;
    أرجع traverse_directed_bfs (غوټه_idx, [&] (
      بینښه پلار,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بینښه ماشوم,
      بینښه _) {

      إذا (link && وموندل.has(پلار))
        // Don't traverse from something that's already marked as a root.
        أرجع خطأ;

      إذا (link && !link->is_signed && link->پلنوالی == 4)
      {
        وموندل.جمع_کړه (link->objidx);
        لیدل_شوی.جمع_کړه (link->objidx);
        أرجع خطأ;
      }

      إذا (لیدل_شوی.has (ماشوم)) أرجع خطأ;
      لیدل_شوی.جمع_کړه (ماشوم);
      أرجع ریښتیا;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  کينډۍ<ډول_نوم O>
  ګراف_پایله_t<بینښه> move_ماشوم (بینښه old_پلار_idx,
                                       ثابت O* old_دننه_واټن,
                                       بینښه new_پلار_idx,
                                       ثابت O* new_دننه_واټن)
  {
    واټن_invalid = ریښتیا;
    positions_invalid = ریښتیا;

    إذا (unlikely (old_پلار_idx >= vertices_.اوږدتیا ||
                  new_پلار_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    خپلکاری& old_v = vertices_[old_پلار_idx];
    خپلکاری& new_v = vertices_[new_پلار_idx];

    TRY_ټاکل (بینښه ماشوم_id, شاخص_for_دننه_واټن (old_پلار_idx,
                                                     old_دننه_واټن));
    إذا (unlikely (ماشوم_id >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    TRY(new_v.جمع_کړه_real_link(O::static_اندازه, ماشوم_id, (ثابت حرف*) new_دننه_واټن - (ثابت حرف*) new_v.obj().سر_2));

    خپلکاری& ماشوم = vertices_[ماشوم_id];
    TRY(ماشوم.جمع_کړه_پلار (new_پلار_idx, خطأ));

    old_v.remove_real_link_unordered (ماشوم_id, old_دننه_واټن);
    ماشوم.remove_پلار (old_پلار_idx);

    أرجع Ok(ماشوم_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  کينډۍ<ډول_نوم O>
  ګراف_پایله_t<فراغ> move_children (بینښه old_پلار_idx,
                                      بینښه old_pos_پیل,
                                      بینښه old_pos_پای_ټکی,
                                      بینښه new_پلار_idx,
                                      بینښه new_pos_پیل)
  {
    إذا (unlikely (old_پلار_idx >= vertices_.اوږدتیا ||
                  new_پلار_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    واټن_invalid = ریښتیا;
    positions_invalid = ریښتیا;

    خپلکاری& old_v = vertices_[old_پلار_idx];
    خپلکاری& new_v = vertices_[new_پلار_idx];

    hb_ویکټور_t<hb_serialize_context_t::object_t::link_t> old_links;
    لكل (ثابت خپلکاری& l : old_v.obj().real_links)
    {
      إذا (l.position < old_pos_پیل || l.position >= old_pos_پای_ټکی)
      {
        old_links.push (l);
        استمر;
      }

      بینښه array_pos = l.position - old_pos_پیل;

      بینښه ماشوم_id = l.objidx;
      إذا (unlikely (ماشوم_id >= vertices_.اوږدتیا))
        أرجع Err(بهر_د_BOUNDS);

      TRY(new_v.جمع_کړه_real_link(O::static_اندازه, ماشوم_id, new_pos_پیل + array_pos));

      خپلکاری& ماشوم = vertices_[ماشوم_id];
      TRY(ماشوم.جمع_کړه_پلار (new_پلار_idx, خطأ));
      ماشوم.remove_پلار (old_پلار_idx);
    }

    TRY (ګراف_پایله_t<فراغ>::from (old_links, ویش_FAILURE));
    أرجع old_v.سېټ_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  ګراف_پایله_t<فراغ> duplicate_subgraph (بینښه غوټه_idx, hb_map_t& شاخص_map)
  {
    ګراف_پایله_t<فراغ> پایله = Ok();
    TRY (traverse_directed_bfs (غوټه_idx, [&] (
      بینښه پلار,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بینښه ماشوم,
      بینښه _) {
      إذا (پایله.is_err () || شاخص_map.has (ماشوم)) أرجع خطأ;
      خپلکاری r = duplicate (ماشوم);
      إذا (unlikely (r.is_err ()))
      {
        پایله = Err(r.error ());
        أرجع خطأ;
      }
      شاخص_map.سېټ (ماشوم, *r);
      أرجع ریښتیا;
    }));

    أرجع ګراف_پایله_t<فراغ>::from(شاخص_map, ویش_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  ګراف_پایله_t<بینښه> duplicate (بینښه غوټه_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (غوټه_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    إذا (unlikely (vertices_.اوږدتیا >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.اوږدتیا);
      أرجع Err(حد_EXCEEDED);
    }

    positions_invalid = ریښتیا;
    واټن_invalid = ریښتیا;

    خپلکاری* clone = vertices_.push ();
    TRY (ګراف_پایله_t<فراغ>::from (vertices_, ویش_FAILURE));

    بینښه clone_idx = vertices_.اوږدتیا - 1;
    ordering_.push (clone_idx);
    TRY (ګراف_پایله_t<فراغ>::from (ordering_, ویش_FAILURE));

    خپلکاری& ماشوم = vertices_[غوټه_idx];

    بینښه جدول_اندازه = ماشوم.obj().لکۍ - ماشوم.obj().سر_2;
    إذا (copy_جدول && جدول_اندازه)
    {
      حرف* بفر = (حرف*) hb_malloc (جدول_اندازه);
      إذا (unlikely (!بفر))
        أرجع Err(ویش_FAILURE);
      خپلکاری res = جمع_کړه_بفر (بفر);
      إذا (unlikely (res.is_err ()))
      {
        hb_free (بفر);
        أرجع Err(res.error ());
      }
      hb_memcpy (بفر, ماشوم.obj().سر_2, جدول_اندازه);
      clone->سېټ_بفر(بفر, بفر + جدول_اندازه);
    }
    وإلا
    {
      clone->سېټ_بفر(ماشوم.obj().سر_2, ماشوم.obj().لکۍ);
    }
    clone->distance = ماشوم.distance;
    clone->space = ماشوم.space;
    clone->reset_parents ();

    لكل (ثابت خپلکاری& l : ماشوم.obj().real_links)
    {
      TRY(clone->جمع_کړه_real_link (l));
      TRY(vertices_[l.objidx].جمع_کړه_پلار (clone_idx, خطأ));
    }

    لكل (ثابت خپلکاری& l : ماشوم.obj().virtual_links)
    {
      TRY(clone->جمع_کړه_virtual_link (l));
      TRY(vertices_[l.objidx].جمع_کړه_پلار (clone_idx, ریښتیا));
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
  ګراف_پایله_t<بینښه> duplicate (بینښه پلار_idx, بینښه ماشوم_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (پلار_idx >= vertices_.اوږدتیا || ماشوم_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    TRY (update_parents ());

    ثابت خپلکاری& ماشوم = vertices_[ماشوم_idx];
    بینښه links_منزل_ماشوم = ماشوم.incoming_څنډې_from_پلار (پلار_idx);

    إذا (unlikely (ماشوم.incoming_څنډې_2 () <= links_منزل_ماشوم || ماشوم.has_incoming_virtual_څنډې_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "  Not duplicating %u => %u",
                 پلار_idx, ماشوم_idx);
      أرجع Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "  Duplicating %u => %u",
               پلار_idx, ماشوم_idx);

    TRY_ټاکل (بینښه clone_idx, duplicate (ماشوم_idx, copy_جدول));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    إذا (پلار_idx == clone_idx) پلار_idx++;

    خپلکاری& پلار = vertices_[پلار_idx];
    بینښه شمېر = 0;
    بینښه عدد_real = پلار.obj().real_links.اوږدتیا;
    لكل (خپلکاری& l : پلار.all_links_writer ())
    {
      شمېر++;
      إذا (l.objidx != ماشوم_idx)
        استمر;

      TRY(reassign_link (l, پلار_idx, clone_idx, شمېر > عدد_real));
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
  ګراف_پایله_t<بینښه> duplicate (ثابت hb_سېټ_t* parents, بینښه ماشوم_idx)
  {
    إذا (unlikely (!parents || parents->is_empty ()))
      أرجع Err(INVALID_دلیل);

    إذا (unlikely (ماشوم_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    TRY (update_parents ());

    ثابت خپلکاری& ماشوم = vertices_[ماشوم_idx];
    بینښه links_منزل_ماشوم = 0;
    بینښه وروستی_پلار = parents->get_max ();
    بینښه لومړی_پلار = parents->get_min ();
    لكل (بینښه پلار_idx : *parents) {
      إذا (unlikely (پلار_idx >= vertices_.اوږدتیا))
        أرجع Err(بهر_د_BOUNDS);
      links_منزل_ماشوم += ماشوم.incoming_څنډې_from_پلار (پلار_idx);
    }

    إذا (unlikely (ماشوم.incoming_څنډې_2 () <= links_منزل_ماشوم || ماشوم.has_incoming_virtual_څنډې_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "  Not duplicating %u, ..., %u => %u", لومړی_پلار, وروستی_پلار, ماشوم_idx);
      أرجع Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "  Duplicating %u, ..., %u => %u", لومړی_پلار, وروستی_پلار, ماشوم_idx);

    TRY_ټاکل (بینښه clone_idx, duplicate (ماشوم_idx));

    لكل (بینښه پلار_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      إذا (پلار_idx == clone_idx) پلار_idx++;
      خپلکاری& پلار = vertices_[پلار_idx];
      بینښه شمېر = 0;
      بینښه عدد_real = پلار.obj().real_links.اوږدتیا;
      لكل (خپلکاری& l : پلار.all_links_writer ())
      {
        شمېر++;
        إذا (l.objidx != ماشوم_idx)
          استمر;

        TRY(reassign_link (l, پلار_idx, clone_idx, شمېر > عدد_real));
      }
    }

    أرجع Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  ګراف_پایله_t<بینښه> new_غوټه (حرف* سر_2, حرف* لکۍ)
  {
    إذا (unlikely (vertices_.اوږدتیا >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.اوږدتیا);
      أرجع Err(حد_EXCEEDED);
    }

    positions_invalid = ریښتیا;
    واټن_invalid = ریښتیا;

    خپلکاری* clone = vertices_.push ();
    TRY (ګراف_پایله_t<فراغ>::from (vertices_, ویش_FAILURE));

    بینښه clone_idx = vertices_.اوږدتیا - 1;
    ordering_.push (clone_idx);
    TRY (ګراف_پایله_t<فراغ>::from (ordering_, ویش_FAILURE));

    clone->سېټ_بفر (سر_2, لکۍ);
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
  ګراف_پایله_t<بینښه> remap_ماشوم (بینښه پلار_idx, بینښه old_ماشوم_idx)
  {
    إذا (unlikely (پلار_idx >= vertices_.اوږدتیا || old_ماشوم_idx >= vertices_.اوږدتیا))
      أرجع Err(بهر_د_BOUNDS);

    TRY_ټاکل (بینښه new_ماشوم_idx, duplicate (old_ماشوم_idx));

    خپلکاری& پلار = vertices_[پلار_idx];
    لكل (خپلکاری& l : پلار.real_links_writer ())
    {
      إذا (l.objidx != old_ماشوم_idx)
        استمر;
      TRY(reassign_link (l, پلار_idx, new_ماشوم_idx, خطأ));
    }

    لكل (خپلکاری& l : پلار.virtual_links_writer ())
    {
      إذا (l.objidx != old_ماشوم_idx)
        استمر;
      TRY(reassign_link (l, پلار_idx, new_ماشوم_idx, ریښتیا));
    }
    أرجع Ok(new_ماشوم_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  ګراف_پایله_t<منطقي> raise_childrens_لومړیتوب (بینښه پلار_idx)
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "  Raising priority of all children of %u",
               پلار_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    إذا (unlikely (پلار_idx >= vertices_.اوږدتیا)) أرجع Err(بهر_د_BOUNDS);
    منطقي made_change = خطأ;
    لكل (خپلکاری& l : vertices_[پلار_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_لومړیتوب ();
    أرجع Ok(made_change);
  }

  ګراف_پایله_t<فراغ> is_fully_connected ()
  {
    TRY(update_parents());

    إذا (unlikely (ریښه().incoming_څنډې_2 ()))
      // Root cannot have parents.
      أرجع Err(ORPHANED_NODES);

    لكل (بینښه i = 0; i < ریښه_idx (); i++)
    {
      إذا (unlikely (!vertices_[i].incoming_څنډې_2 ()))
        أرجع Err(ORPHANED_NODES);
    }
    أرجع Ok();
  }

#إذا 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  فراغ save_fuzzer_تخم (hb_نښان_t نښان_2) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((فراغ*) &نښان_2, حجم (نښان_2), 1, f);

    uint16_t عدد_objects = vertices_.اوږدتیا;
    fwrite ((فراغ*) &عدد_objects, حجم (عدد_objects), 1, f);

    لكل (ثابت خپلکاری& v : vertices_)
    {
      uint16_t blob_اندازه = v.جدول_اندازه ();
      fwrite ((فراغ*) &blob_اندازه, حجم (blob_اندازه), 1, f);
      fwrite ((ثابت فراغ*) v.obj.سر_2, blob_اندازه, 1, f);
    }

    uint16_t link_شمېر = 0;
    لكل (ثابت خپلکاری& v : vertices_)
      link_شمېر += v.obj.real_links.اوږدتیا;

    fwrite ((فراغ*) &link_شمېر, حجم (link_شمېر), 1, f);

    تعريفنوع بنية
    {
      uint16_t پلار;
      uint16_t ماشوم;
      uint16_t position;
      uint8_t پلنوالی;
    } link_t;

    لكل (بینښه i = 0; i < vertices_.اوږدتیا; i++)
    {
      لكل (ثابت خپلکاری& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.پلنوالی
        };
        fwrite ((فراغ*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#د_که_پای

  بینښه عدد_roots_for_space_2 (بینښه space) ثابت
  {
    أرجع عدد_roots_for_space[space];
  }

  بینښه بل_space () ثابت
  {
    أرجع عدد_roots_for_space.اوږدتیا;
  }

  ګراف_پایله_t<فراغ> move_منزل_new_space (ثابت hb_سېټ_t& indices)
  {
    إذا (unlikely (عدد_roots_for_space.اوږدتیا >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 عدد_roots_for_space.اوږدتیا);
      أرجع Err(حد_EXCEEDED);
    }
    عدد_roots_for_space.push (0);
    TRY (ګراف_پایله_t<فراغ>::from (عدد_roots_for_space, ویش_FAILURE));
    بینښه new_space = عدد_roots_for_space.اوږدتیا - 1;

    لكل (بینښه شاخص_2 : indices) {
      إذا (unlikely (شاخص_2 >= vertices_.اوږدتیا))
        أرجع Err(بهر_د_BOUNDS);
      خپلکاری& node = vertices_[شاخص_2];
      عدد_roots_for_space[node.space] = عدد_roots_for_space[node.space] - 1;
      عدد_roots_for_space[new_space] = عدد_roots_for_space[new_space] + 1;
      node.space = new_space;
      واټن_invalid = ریښتیا;
      positions_invalid = ریښتیا;
    }
    أرجع Ok();
  }

  بینښه space_for (بینښه شاخص_2, بینښه* ریښه = تش_اشاره) ثابت
  {
    طالما (ریښتیا)
    {
      أكد (شاخص_2 < vertices_.اوږدتیا);
      ثابت خپلکاری& node = vertices_[شاخص_2];
      إذا (node.space)
      {
        إذا (ریښه != تش_اشاره)
          *ریښه = شاخص_2;
        أرجع node.space;
      }

      إذا (!node.incoming_څنډې_2 ())
      {
        إذا (ریښه)
          *ریښه = شاخص_2;
        أرجع 0;
      }

      شاخص_2 = *node.parents_iter ();
    }
  }

  size_t ټول_اندازه_دننه_بایټونه () ثابت {
    size_t ټول_اندازه = 0;
    بینښه شمېر = vertices_.اوږدتیا;
    لكل (بینښه i = 0; i < شمېر; i++) {
      size_t اندازه = vertices_.arrayZ[i].جدول_اندازه ();
      ټول_اندازه += اندازه;
    }
    أرجع ټول_اندازه;
  }


 خاص:

  فراغ print_orphaned_nodes ()
  {
    إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Graph is not fully connected.");

    parents_invalid = ریښتیا;
    (فراغ) update_parents();

    إذا (ریښه().incoming_څنډې_2 ()) {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Root node has incoming edges.");
    }

    لكل (بینښه i = 0; i < vertices_.اوږدتیا; i++)
    {
      ثابت خپلکاری& v = vertices_[i];
      إذا (!v.incoming_څنډې_2 ())
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  بینښه wide_parents (بینښه غوټه_idx, hb_سېټ_t& parents) ثابت
  {
    بینښه شمېر = 0;
    لكل (بینښه p : vertices_[غوټه_idx].parents_iter ())
    {
      // Only real links can be wide
      لكل (ثابت خپلکاری& l : vertices_[p].obj().real_links)
      {
        إذا (l.objidx == غوټه_idx
            && (l.پلنوالی == 3 || l.پلنوالی == 4)
            && !l.is_signed)
        {
          شمېر++;
          parents.جمع_کړه (p);
        }
      }
    }
    أرجع شمېر;
  }

 عام:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  ګراف_پایله_t<فراغ> update_parents ()
  {
    إذا (!parents_invalid) أرجع Ok();

    بینښه شمېر = vertices_.اوږدتیا;

    لكل (بینښه i = 0; i < شمېر; i++)
      vertices_.arrayZ[i].reset_parents ();

    لكل (بینښه p = 0; p < شمېر; p++)
    {
      لكل (ثابت خپلکاری& l : vertices_.arrayZ[p].obj().real_links)
      {
        إذا (unlikely (l.objidx >= شمېر)) أرجع Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جمع_کړه_پلار (p, خطأ));
      }

      لكل (ثابت خپلکاری& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        إذا (unlikely (l.objidx >= شمېر)) أرجع Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جمع_کړه_پلار (p, ریښتیا));
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

    بینښه current_pos = 0;
    لكل (بینښه i : ordering_)
    {
      خپلکاری& v = vertices_[i];
      v.پیل = current_pos;
      current_pos += v.obj().لکۍ - v.obj().سر_2;
      v.پای_ټکی = current_pos;
    }

    positions_invalid = خطأ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  ګراف_پایله_t<فراغ> update_distances ()
  {
    إذا (!واټن_invalid) أرجع Ok();

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
    بینښه شمېر = vertices_.اوږدتیا;
    لكل (بینښه i = 0; i < شمېر; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[ریښه_idx ()].distance = 0;

    hb_لومړیتوب_کتار_t<int64_t> کتار;
    کتار.alloc (شمېر);
    کتار.insert (0, ریښه_idx ());
    TRY (ګراف_پایله_t<فراغ>::from (کتار, ویش_FAILURE));

    hb_ویکټور_t<منطقي> لیدل_شوی;
    لیدل_شوی.resize (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (لیدل_شوی, ویش_FAILURE));

    طالما (!کتار.دننه_error () && !کتار.is_empty ())
    {
      بینښه بل_idx = کتار.راوباسه_minimum ().second;
      إذا (لیدل_شوی[بل_idx]) استمر;
      ثابت خپلکاری& next = vertices_[بل_idx];
      int64_t بل_واټن = next.distance;
      لیدل_شوی[بل_idx] = ریښتیا;

      لكل (ثابت خپلکاری& link : next.obj().all_links ())
      {
        إذا (unlikely (link.objidx >= vertices_.اوږدتیا))
          أرجع Err(بهر_د_BOUNDS);

        إذا (لیدل_شوی[link.objidx]) استمر;

        خپلکاری& ماشوم_v = vertices_.arrayZ[link.objidx];
        ثابت خپلکاری& ماشوم = ماشوم_v.obj ();
        بینښه link_پلنوالی = link.پلنوالی ? link.پلنوالی : 4; // treat virtual offsets as 32 bits wide
        int64_t ماشوم_وزن = (ماشوم.لکۍ - ماشوم.سر_2) +
                               ((int64_t) 1 << (link_پلنوالی * 8)) * (ماشوم_v.space + 1);
        int64_t ماشوم_واټن = بل_واټن + ماشوم_وزن;

        إذا (ماشوم_واټن < ماشوم_v.distance)
        {
          ماشوم_v.distance = ماشوم_واټن;
          کتار.insert (ماشوم_واټن, link.objidx);
        }
      }
    }

    TRY (ګراف_پایله_t<فراغ>::from (کتار, ویش_FAILURE));
    إذا (unlikely (!کتار.is_empty ()))
    {
      print_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    واټن_invalid = خطأ;
    أرجع Ok();
  }

 خاص:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  ګراف_پایله_t<فراغ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      بینښه پلار_idx,
                      بینښه new_idx,
                      منطقي is_virtual)
  {
    بینښه old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_پلار (پلار_idx);
    أرجع vertices_[new_idx].جمع_کړه_پلار (پلار_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  کينډۍ<ډول_نوم Iterator, hb_requires (hb_is_iterator (Iterator))>
  ګراف_پایله_t<فراغ> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقي only_wide = خطأ)
  {
    إذا (!id_map) أرجع Ok();
    لكل (بینښه i : subgraph)
    {
      خپلکاری& راس = vertices_[i];
      بینښه عدد_real = راس.obj().real_links.اوږدتیا;
      بینښه شمېر = 0;
      لكل (خپلکاری& link : راس.all_links_writer ())
      {
        شمېر++;
        ثابت uint32_t *v;
        إذا (!id_map.has (link.objidx, &v)) استمر;
        إذا (only_wide && (link.is_signed || (link.پلنوالی != 4 && link.پلنوالی != 3))) استمر;

        TRY(reassign_link (link, i, *v, شمېر > عدد_real));
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
  ګراف_پایله_t<فراغ> پیدا_کړه_connected_nodes (بینښه پیل_idx,
                                             hb_سېټ_t& targets,
                                             hb_سېټ_t& لیدل_شوی,
                                             hb_سېټ_t& connected)
  {
    TRY (ګراف_پایله_t<فراغ>::from (لیدل_شوی, ویش_FAILURE));
    إذا (لیدل_شوی.has (پیل_idx)) أرجع Ok();
    إذا (unlikely (پیل_idx >= vertices_.اوږدتیا))
    {
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      أرجع Err(بهر_د_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.اوږدتیا);
    TRY (ګراف_پایله_t<فراغ>::from (ordering_scratch_, ویش_FAILURE));

    خپلکاری& ډېری = ordering_scratch_;
    بینښه ډېری_len = 0;

    ګراف_پایله_t<فراغ> res = Ok();
    خپلکاری handle_غوټه = [&] (بینښه غوټه_idx) {
      لیدل_شوی.جمع_کړه (غوټه_idx);
      إذا (targets.has (غوټه_idx)) {
        targets.del (غوټه_idx);
        connected.جمع_کړه (غوټه_idx);
      }

      إذا (unlikely (ډېری_len >= ډېری.اوږدتیا))
      {
        res = Err(دوره_DETECTED);
        أرجع خطأ;
      }
      ډېری[ډېری_len++] = غوټه_idx;
      أرجع ریښتیا;
    };

    إذا (!handle_غوټه (پیل_idx)) أرجع res;

    طالما (ډېری_len > 0)
    {
      بینښه غوټه_idx = ډېری[--ډېری_len];
      ثابت خپلکاری& v = vertices_[غوټه_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      لكل (ثابت خپلکاری& l : v.obj().all_links ())
      {
        بینښه ماشوم_idx = l.objidx;
        إذا (لیدل_شوی.has (ماشوم_idx)) استمر;
        إذا (!handle_غوټه (ماشوم_idx)) أرجع res;
      }

      لكل (بینښه پلار_idx : v.parents_iter ())
      {
        إذا (لیدل_شوی.has (پلار_idx)) استمر;
        إذا (!handle_غوټه (پلار_idx)) أرجع res;
      }
    }

    TRY (ګراف_پایله_t<فراغ>::from (لیدل_شوی, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (connected, ویش_FAILURE));

    أرجع Ok();
  }

 عام:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_ویکټور_t<راس_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_ویکټور_t<بینښه> ordering_;
  hb_ویکټور_t<بینښه> ordering_scratch_;

 خاص:
  منطقي parents_invalid = ریښتیا;
  منطقي واټن_invalid = ریښتیا;
  منطقي positions_invalid = ریښتیا;
  hb_ویکټور_t<بینښه> عدد_roots_for_space;
  hb_ویکټور_t<حرف*> buffers;
};

}

#د_که_پای  // GRAPH_GRAPH_HH
