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

#شامل "../hb-set.hh"
#شامل "../hb-priority-queue.hh"
#شامل "../hb-serialize.hh"
#شامل "graph-result.hh"

#اگر_غیر_تعریف_شدہ گراف_گراف_HH
#تعریف_کرو گراف_گراف_HH

#تعریف_کرو HB_گراف_INVALID ((غیرعلامتی) -1)

نامخلا گراف {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
ساخت گراف_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  ساخت راس_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    غیرعلامتی space = 0 ;
    غیرعلامتی آغاز = 0;
    غیرعلامتی انتہا = 0;
    غیرعلامتی ترجیح = 0;

    نجی:
    hb_serialize_context_t::object_t obj_;
    غیرعلامتی incoming_کنارے = 0;
    غیرعلامتی single_والد = HB_گراف_INVALID;
    منطقی has_incoming_virtual_کنارے = جھوٹ;
    hb_hashmap_t<غیرعلامتی, غیرعلامتی> parents;
    hb_سیٹ_t virtual_parents;
    عوامی:

    راس_t ()
    {}

    صریح راس_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    خودکار parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_والد, single_والد != HB_گراف_INVALID),
	parents.کلیدیں_ref ()
      )
    )

    خالی سیٹ_بفر (حرف* سر_2, حرف* دم) {
      obj_.سر_2 = سر_2;
      obj_.دم = دم;
    }

    خالی shrink_بفر (size_t گنتی)
    {
      obj_.دم -= گنتی;
    }

    خودکار all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    خودکار real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    خودکار virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    خالی ترتیب_دو_real_links () {
      obj_.real_links.qsort ();
    }

    گراف_نتیجہ_t<خالی> جوڑو_real_link (غیرعلامتی چوڑائی, غیرعلامتی obj_idx, غیرعلامتی position) {
      خودکار* link = obj_.real_links.push ();
      TRY (گراف_نتیجہ_t<خالی>::from (obj_.real_links, تخصیص_FAILURE));
      link->چوڑائی = چوڑائی;
      link->objidx = obj_idx;
      link->position = position;
      واپس Ok();
    }

    گراف_نتیجہ_t<خالی> جوڑو_virtual_link (غیرعلامتی obj_idx) {
      خودکار* link = obj_.virtual_links.push ();
      TRY (گراف_نتیجہ_t<خالی>::from (obj_.virtual_links, تخصیص_FAILURE));
      link->objidx = obj_idx;
      واپس Ok();
    }

    گراف_نتیجہ_t<خالی> جوڑو_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      واپس گراف_نتیجہ_t<خالی>::from (obj_.real_links, تخصیص_FAILURE);
    }

    گراف_نتیجہ_t<خالی> سیٹ_real_links (hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(گراف_نتیجہ_t<خالی>::from (links, تخصیص_FAILURE));
      obj_.real_links = links;
      واپس Ok();
    }

    خالی clear_real_links ()
    {
      obj_.real_links.reset();
    }

    خالی clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    گراف_نتیجہ_t<خالی> جوڑو_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      واپس گراف_نتیجہ_t<خالی>::from (obj_.virtual_links, تخصیص_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { واپس obj_; }

    سانچہ<قسم_نام T>
    T as () ثابت
    {
      واپس reinterpret_cast<T> (obj_.سر_2);
    }

    منطقی has_incoming_virtual_کنارے_2 () ثابت
    {
      واپس has_incoming_virtual_کنارے;
    }

    گراف_نتیجہ_t<خالی> link_positions_valid (غیرعلامتی عدد_objects, منطقی removed_nil)
    {
      hb_سیٹ_t assigned_بائٹیں;
      برائے (ثابت خودکار& l : obj_.real_links)
      {
        اگر (unlikely (l.objidx >= عدد_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                     "Invalid graph. Invalid object index.");
          واپس Err(INVALID_دلیل);
        }

        غیرعلامتی آغاز = l.position;
        غیرعلامتی انتہا = آغاز + l.چوڑائی - 1;

        اگر (unlikely (l.چوڑائی < 2 || l.چوڑائی > 4))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                     "Invalid graph. Invalid link width.");
          واپس Err(INVALID_دلیل);
        }

        اگر (unlikely (انتہا >= جدول_سائز ()))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                     "Invalid graph. Link position is out of bounds.");
          واپس Err(برآمد_کا_BOUNDS);
        }

        اگر (unlikely (assigned_بائٹیں.intersects (آغاز, انتہا)))
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                     "Invalid graph. Found offsets whose positions overlap.");
          واپس Err(INVALID_دلیل);
        }

        assigned_بائٹیں.جوڑو_range (آغاز, انتہا);
      }

      واپس گراف_نتیجہ_t<خالی>::from (assigned_بائٹیں, تخصیص_FAILURE);
    }

    خالی normalize ()
    {
      obj_.real_links.qsort ();
      برائے (ثابت خودکار& l : obj_.real_links)
      {
        برائے (غیرعلامتی i = 0; i < l.چوڑائی; i++)
        {
          obj_.سر_2[l.position + i] = 0;
        }
      }
    }

    منطقی equals (غیرعلامتی this_اشاریہ,
                 غیرعلامتی other_اشاریہ,
                 ثابت راس_t& other,
                 ثابت گراف_t& گراف,
                 ثابت گراف_t& other_گراف,
                 غیرعلامتی گہرائی) ثابت
    {
      اگر (!(as_بائٹیں () == other.as_بائٹیں ()))
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_اشاریہ,
                   (غیرعلامتی طویل) جدول_سائز (),
                   other_اشاریہ,
                   (غیرعلامتی طویل) other.جدول_سائز (),
                   گہرائی);

        خودکار a = as_بائٹیں ();
        خودکار b = other.as_بائٹیں ();
        جب (a || b)
        {
          DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                     "  0x%x %s 0x%x", (غیرعلامتی) *a, (*a == *b) ? "==" : "!=", (غیرعلامتی) *b);
          a++;
          b++;
        }
        واپس جھوٹ;
      }

      واپس links_equal (obj_.real_links, other.obj_.real_links, گراف, other_گراف, گہرائی);
    }

    hb_بائٹیں_t as_بائٹیں () ثابت
    {
      واپس hb_بائٹیں_t (obj_.سر_2, جدول_سائز ());
    }

    دوست خالی swap (راس_t& a, راس_t& b)
    {
      hb_تبادلہ (a.obj_, b.obj_);
      hb_تبادلہ (a.distance, b.distance);
      hb_تبادلہ (a.space, b.space);
      hb_تبادلہ (a.single_والد, b.single_والد);
      hb_تبادلہ (a.parents, b.parents);
      hb_تبادلہ (a.virtual_parents, b.virtual_parents);
      hb_تبادلہ (a.incoming_کنارے, b.incoming_کنارے);
      hb_تبادلہ (a.has_incoming_virtual_کنارے, b.has_incoming_virtual_کنارے);
      hb_تبادلہ (a.آغاز, b.آغاز);
      hb_تبادلہ (a.انتہا, b.انتہا);
      hb_تبادلہ (a.ترجیح, b.ترجیح);
    }

    گراف_نتیجہ_t<hb_hashmap_t<غیرعلامتی, غیرعلامتی>>
    position_مقصد_اشاریہ_map () ثابت
    {
      hb_hashmap_t<غیرعلامتی, غیرعلامتی> نتیجہ;

      نتیجہ.alloc (obj_.real_links.لمبائی);
      برائے (ثابت خودکار& l : obj_.real_links) {
        نتیجہ.سیٹ (l.position, l.objidx);
      }

      واپس گراف_نتیجہ_t<hb_hashmap_t<غیرعلامتی, غیرعلامتی>>::from (std::move(نتیجہ), تخصیص_FAILURE);
    }

    منطقی is_shared () ثابت
    {
      واپس parents.get_population () > virtual_parents.get_population () + 1;
    }

    غیرعلامتی incoming_کنارے_2 () ثابت
    {
      اگر (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_کنارے == (single_والد != HB_گراف_INVALID) +
		(parents.اقدار_ref () | hb_reduce (hb_جوڑو, 0)));
       }
      واپس incoming_کنارے;
    }

    غیرعلامتی incoming_کنارے_from_والد (غیرعلامتی والد_اشاریہ) ثابت {
      اگر (single_والد != HB_گراف_INVALID) {
        واپس single_والد == والد_اشاریہ ? 1 : 0;
      }

      غیرعلامتی* گنتی;
      واپس  parents.has(والد_اشاریہ, &گنتی) ? *گنتی : 0;
    }

    خالی reset_parents ()
    {
      incoming_کنارے = 0;
      has_incoming_virtual_کنارے = جھوٹ;
      single_والد = HB_گراف_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    گراف_نتیجہ_t<خالی> جوڑو_والد (غیرعلامتی والد_اشاریہ, منطقی is_virtual)
    {
      أكد (والد_اشاریہ != HB_گراف_INVALID);
      has_incoming_virtual_کنارے |= is_virtual;
      اگر (is_virtual)
        virtual_parents.جوڑو (والد_اشاریہ);

      اگر (incoming_کنارے == 0)
      {
	single_والد = والد_اشاریہ;
	incoming_کنارے = 1;
	واپس مقصد_نتیجہ();
      }
      ورنہ اگر (single_والد != HB_گراف_INVALID)
      {
        أكد (incoming_کنارے == 1);
	اگر (!parents.سیٹ (single_والد, 1))
	  واپس مقصد_نتیجہ ();
	single_والد = HB_گراف_INVALID;
      }

      غیرعلامتی *v;
      اگر (parents.has (والد_اشاریہ, &v))
      {
        (*v)++;
	incoming_کنارے++;
      }
      ورنہ اگر (parents.سیٹ (والد_اشاریہ, 1))
	incoming_کنارے++;

      واپس مقصد_نتیجہ();
    }

    خالی remove_والد (غیرعلامتی والد_اشاریہ)
    {
      اگر (والد_اشاریہ == single_والد)
      {
	single_والد = HB_گراف_INVALID;
	incoming_کنارے--;
	virtual_parents.reset ();
	واپس;
      }

      غیرعلامتی *v;
      اگر (parents.has (والد_اشاریہ, &v))
      {
	incoming_کنارے--;
	اگر (*v > 1)
	  (*v)--;
	ورنہ
        {
	  parents.del (والد_اشاریہ);
          virtual_parents.del (والد_اشاریہ);
        }

	اگر (incoming_کنارے == 1)
	{
	  single_والد = *parents.کلیدیں ();
	  parents.reset ();
	}
      }
    }

    خالی remove_real_link_unordered (غیرعلامتی link_array_اشاریہ) {
      obj_.real_links.remove_unordered (link_array_اشاریہ);
    }

    خالی remove_real_link_unordered (غیرعلامتی بچہ_اشاریہ, ثابت خالی* انحراف)
    {
      غیرعلامتی گنتی = obj_.real_links.لمبائی;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
        خودکار& link = obj_.real_links.arrayZ[i];
        اگر (link.objidx != بچہ_اشاریہ)
          جاری;

        اگر ((obj_.سر_2 + link.position) != انحراف)
          جاری;

        obj_.real_links.remove_unordered (i);
        واپس;
      }
    }

    گراف_نتیجہ_t<خالی> remap_والد (غیرعلامتی old_اشاریہ, غیرعلامتی new_اشاریہ)
    {
      اگر (single_والد != HB_گراف_INVALID)
      {
        اگر (single_والد == old_اشاریہ)
        {
	  single_والد = new_اشاریہ;
          اگر (virtual_parents.has (old_اشاریہ))
          {
            virtual_parents.del (old_اشاریہ);
            virtual_parents.جوڑو (new_اشاریہ);
          }
        }
        واپس گراف_نتیجہ_t<خالی>::from(virtual_parents, تخصیص_FAILURE);
      }

      ثابت غیرعلامتی *pv;
      اگر (parents.has (old_اشاریہ, &pv))
      {
        غیرعلامتی v = *pv;
	اگر (!parents.سیٹ (new_اشاریہ, v))
          incoming_کنارے -= v;
	parents.del (old_اشاریہ);

        اگر (incoming_کنارے == 1)
	{
	  single_والد = *parents.کلیدیں ();
	  parents.reset ();
	}
      }

      اگر (virtual_parents.has (old_اشاریہ))
      {
        virtual_parents.del (old_اشاریہ);
        virtual_parents.جوڑو (new_اشاریہ);
      }

      واپس گراف_نتیجہ_t<خالی>::from(virtual_parents, تخصیص_FAILURE);
    }

    منطقی is_برگ () ثابت
    {
      واپس !obj_.real_links.لمبائی && !obj_.virtual_links.لمبائی;
    }

    منطقی raise_ترجیح ()
    {
      اگر (has_max_ترجیح ()) واپس جھوٹ;
      ترجیح++;
      واپس سچ;
    }

    منطقی give_max_ترجیح ()
    {
      منطقی نتیجہ = جھوٹ;
      جب (!has_max_ترجیح()) {
        نتیجہ = سچ;
        ترجیح++;
      }
      واپس نتیجہ;
    }

    منطقی has_max_ترجیح () ثابت {
      واپس ترجیح >= 3;
    }

    size_t جدول_سائز () ثابت {
      واپس obj_.دم - obj_.سر_2;
    }

    int64_t modified_فاصلہ (غیرعلامتی ترتیب) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_فاصلہ =
          hb_clamp (distance + فاصلہ_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      اگر (has_max_ترجیح ()) {
        modified_فاصلہ = 0;
      }
      واپس (modified_فاصلہ << 18) | (0x003FFFF & ترتیب);
    }

    int64_t فاصلہ_modifier () ثابت
    {
      اگر (!ترجیح) واپس 0;
      int64_t جدول_سائز = obj_.دم - obj_.سر_2;

      اگر (ترجیح == 1)
        واپس -جدول_سائز / 2;

      واپس -جدول_سائز;
    }

   نجی:
    گراف_نتیجہ_t<خالی> مقصد_نتیجہ() ثابت {
      اگر (unlikely (
        parents.اندر_error() || virtual_parents.اندر_error() ||
        obj_.real_links.اندر_error() || obj_.virtual_links.اندر_error()))
        واپس Err(تخصیص_FAILURE);
      واپس Ok();
    }

    منطقی links_equal (ثابت hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت گراف_t& گراف,
                      ثابت گراف_t& other_گراف,
                      غیرعلامتی گہرائی) ثابت
    {
      خودکار a = this_links.iter ();
      خودکار b = other_links.iter ();

      جب (a && b)
      {
        ثابت خودکار& link_a = *a;
        ثابت خودکار& link_b = *b;

        اگر (link_a.چوڑائی != link_b.چوڑائی ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          واپس جھوٹ;

        اگر (!گراف.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_گراف.vertices_[link_b.objidx], گراف, other_گراف, گہرائی + 1))
          واپس جھوٹ;

        a++;
        b++;
      }

      اگر (منطقی (a) != منطقی (b))
        واپس جھوٹ;

      واپس سچ;
    }
  };

  شمار راس_mutability_t
  {
    Immutable,
    Mutable
  };

  سانچہ <قسم_نام T, راس_mutability_t mutability = Mutable>
  ساخت راس_and_جدول_t
  {
    راس_and_جدول_t () : اشاریہ_2 (0), راس (خالی_اشاریہ), جدول (خالی_اشاریہ)
    {}

    سانچہ <راس_mutability_t other_mutability>
    راس_and_جدول_t (ثابت راس_and_جدول_t<T, other_mutability>& o)
      : اشاریہ_2 (o.اشاریہ_2), راس (o.راس), جدول (o.جدول)
    {}

    غیرعلامتی اشاریہ_2;
    قسم_نام std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type راس;
    قسم_نام std::conditional<mutability == Immutable, ثابت T*, T*>::type جدول;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  سانچہ<قسم_نام T>
  ساکن گراف_نتیجہ_t<گراف_t> create (ثابت T& objects)
  {
    اگر (unlikely (objects.لمبائی > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.لمبائی);
      واپس Err(حد_بندی_EXCEEDED);
    }

    گراف_t g;
    g.عدد_roots_for_space.push (1);
    TRY (گراف_نتیجہ_t<خالی>::from (g.عدد_roots_for_space, تخصیص_FAILURE));

    منطقی removed_nil = جھوٹ;
    ثابت غیرعلامتی گنتی = objects.لمبائی;
    g.vertices_.alloc (گنتی);
    g.ordering_.resize (گنتی);
    g.ordering_scratch_.alloc (گنتی);
    TRY (گراف_نتیجہ_t<خالی>::from (g.vertices_, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (g.ordering_, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (g.ordering_scratch_, تخصیص_FAILURE));

    غیرعلامتی ترتیب = objects.لمبائی;
    غیرعلامتی skip = 0;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      اگر (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = سچ;
        ترتیب--;
        g.ordering_.resize (گنتی - 1);
        skip++;
        جاری;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      اگر (unlikely (obj.اندر_error ())) {
        واپس Err(تخصیص_FAILURE);
      }
      راس_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (گنتی, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      غیرعلامتی obj_idx = i - skip;
      g.ordering_[--ترتیب] = obj_idx;

      اگر (!removed_nil) جاری;
      // Fix indices to account for removed nil object.
      برائے (خودکار& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    خودکار r = g.is_fully_connected();
    اگر (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    واپس Ok (std::move (g));
  }

  گراف_t (ثابت گراف_t&) = حذف;
  گراف_t& عامل = (ثابت گراف_t&) = حذف;
  گراف_t (گراف_t&&) = طےشدہ;
  گراف_t& عامل = (گراف_t&&) = طےشدہ;

  ~گراف_t ()
  {
    برائے (حرف* b : buffers)
      hb_free (b);
  }

نجی:
  گراف_t () = طےشدہ;

عوامی:

  منطقی عامل== (ثابت گراف_t& other) ثابت
  {
    واپس جڑ ().equals (جڑ_idx(), other.جڑ_idx(), other.جڑ (), *یہ, other, 0);
  }

  خالی print () ثابت {
    برائے (غیرعلامتی id : ordering_)
    {
      ثابت خودکار& v = vertices_[id];
      لکھو("%u: %u [", id, (غیرعلامتی صحیح)v.جدول_سائز());
      برائے (ثابت خودکار &l : v.obj().real_links) {
        لکھو("%u, ", l.objidx);
      }
      برائے (ثابت خودکار &l : v.obj().virtual_links) {
        لکھو("v%u, ", l.objidx);
      }
      لکھو("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  خالی normalize ()
  {
    برائے (خودکار& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت راس_t& جڑ () ثابت
  {
    واپس vertices_[جڑ_idx ()];
  }

  غیرعلامتی جڑ_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    واپس ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (غیرعلامتی i) ثابت
  {
    واپس vertices_[i].obj();
  }

  گراف_نتیجہ_t<خالی> جوڑو_بفر (حرف* بفر)
  {
    buffers.push (بفر);
    واپس گراف_نتیجہ_t<خالی>::from (buffers, تخصیص_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  سانچہ<قسم_نام T>
  گراف_نتیجہ_t<خالی> جوڑو_link (T* انحراف,
                                 غیرعلامتی والد_id,
                                 غیرعلامتی بچہ_id)
  {
    اگر (unlikely (والد_id >= vertices_.لمبائی || بچہ_id >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);
    خودکار& v = vertices_[والد_id];
    TRY(v.جوڑو_real_link(2, بچہ_id, (حرف*) انحراف - (حرف*) v.obj().سر_2));
    واپس vertices_[بچہ_id].جوڑو_والد (والد_id, جھوٹ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  گراف_نتیجہ_t<خالی> ترتیب_دو_مختصر_ترین_فاصلہ_if_needed ()
  {
    اگر (!positions_invalid) واپس Ok();
    واپس ترتیب_دو_مختصر_ترین_فاصلہ ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  گراف_نتیجہ_t<خالی> ترتیب_دو_مختصر_ترین_فاصلہ ()
  {
    positions_invalid = سچ;

    اگر (vertices_.لمبائی <= 1) {
      // Graph of 1 or less doesn't need sorting.
      واپس Ok();
    }

    TRY (update_distances ());

    hb_ترجیح_قطار_t<int64_t> قطار;
    قطار.alloc (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (قطار, تخصیص_FAILURE));

    hb_سمتیہ_t<غیرعلامتی> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (new_ordering, تخصیص_FAILURE));

    hb_سمتیہ_t<غیرعلامتی> removed_کنارے;
    removed_کنارے.resize (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (removed_کنارے, تخصیص_FAILURE));

    TRY (update_parents ());

    قطار.insert (جڑ ().modified_فاصلہ (0), جڑ_idx ());
    غیرعلامتی ترتیب = 1;
    غیرعلامتی pos = 0;
    جب (!قطار.اندر_error () && !قطار.is_empty ())
    {
      غیرعلامتی اگلا_id = قطار.نکالو_minimum().second;

      اگر (unlikely (pos >= new_ordering.لمبائی)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Invalid graph. Contains cycle.");
        واپس Err(چکر_DETECTED);
      }
      new_ordering[pos++] = اگلا_id;
      ثابت راس_t& next = vertices_[اگلا_id];

      برائے (ثابت خودکار& link : next.obj().all_links ()) {
        اگر (unlikely (link.objidx >= vertices_.لمبائی))
          واپس Err(برآمد_کا_BOUNDS);
        removed_کنارے[link.objidx]++;
        ثابت خودکار& v = vertices_[link.objidx];
        اگر (!(v.incoming_کنارے_2 () - removed_کنارے[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          قطار.insert (v.modified_فاصلہ (ترتیب++),
                        link.objidx);
      }
    }

    TRY (گراف_نتیجہ_t<خالی>::from (قطار, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (new_ordering, تخصیص_FAILURE));

    hb_تبادلہ (ordering_, new_ordering);

    اگر (unlikely (pos != vertices_.لمبائی)) {
      print_orphaned_nodes ();
      واپس Err(ORPHANED_NODES);
    }

    واپس Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  گراف_نتیجہ_t<خالی> ڈھونڈو_space_roots (hb_سیٹ_t& دیکھا, hb_سیٹ_t& roots)
  {
    غیرعلامتی جڑ_اشاریہ = جڑ_idx ();
    برائے (غیرعلامتی i : ordering_)
    {
      اگر (دیکھا.has (i)) جاری;

      // Only real links can form 32 bit spaces
      برائے (ثابت خودکار& l : vertices_[i].obj().real_links)
      {
        اگر (l.is_signed || l.چوڑائی < 3)
          جاری;

        اگر (i == جڑ_اشاریہ && l.چوڑائی == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          جاری;

        اگر (l.چوڑائی >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_سیٹ_t sub_roots;
          TRY (ڈھونڈو_32bit_roots (l.objidx, sub_roots));
          اگر (sub_roots) {
            برائے (غیرعلامتی sub_جڑ_idx : sub_roots) {
              roots.جوڑو (sub_جڑ_idx);
              TRY (ڈھونڈو_subgraph (sub_جڑ_idx, دیکھا));
            }
            جاری;
          }
        }

        roots.جوڑو (l.objidx);
        TRY (ڈھونڈو_subgraph (l.objidx, دیکھا));
      }
    }

    TRY(گراف_نتیجہ_t<خالی>::from (دیکھا, تخصیص_FAILURE));
    TRY(گراف_نتیجہ_t<خالی>::from (roots, تخصیص_FAILURE));
    واپس Ok();
  }

  سانچہ <قسم_نام T, قسم_نام ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, Immutable>> as_جدول (غیرعلامتی والد, ثابت خالی* انحراف, Ts... ds)
  {
    TRY_تفویض (غیرعلامتی idx, اشاریہ_for_انحراف (والد, انحراف));
    واپس as_جدول_from_اشاریہ<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچہ <قسم_نام T, قسم_نام ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, Mutable>> as_mutable_جدول (غیرعلامتی والد, ثابت خالی* انحراف, Ts... ds)
  {
    TRY_تفویض (خودکار idx, mutable_اشاریہ_for_انحراف (والد, انحراف));
    واپس as_جدول_from_اشاریہ<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  سانچہ <قسم_نام T, راس_mutability_t mutability = Mutable, قسم_نام ...Ts>
  گراف_نتیجہ_t<راس_and_جدول_t<T, mutability>> as_جدول_from_اشاریہ (غیرعلامتی اشاریہ_2, Ts... ds)
  {
    اگر (unlikely (اشاریہ_2 >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    راس_and_جدول_t<T, mutability> r;
    r.راس = (قسم_نام std::conditional<mutability == Immutable, ثابت راس_t*, راس_t*>::type) &vertices_[اشاریہ_2];
    r.جدول = r.راس->سانچہ as<قسم_نام std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.اشاریہ_2 = اشاریہ_2;
    اگر (unlikely (!r.جدول))
      واپس Err(INVALID_دلیل);

    TRY(r.جدول->sanitize (*(r.راس), std::forward<Ts>(ds)...));
    واپس r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  گراف_نتیجہ_t<غیرعلامتی> اشاریہ_for_انحراف (غیرعلامتی گرہ_idx, ثابت خالی* انحراف) ثابت
  {
    اگر (unlikely (گرہ_idx >= vertices_.لمبائی)) واپس Err(برآمد_کا_BOUNDS);
    ثابت خودکار& node = object (گرہ_idx);
    اگر (unlikely (انحراف < node.سر_2 || انحراف >= node.دم)) واپس Err(برآمد_کا_BOUNDS);

    غیرعلامتی گنتی = node.real_links.لمبائی;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت خودکار& link = node.real_links.arrayZ[i];
      اگر (انحراف != node.سر_2 + link.position)
        جاری;
      واپس Ok(link.objidx);
    }

    واپس Err(INVALID_دلیل);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  گراف_نتیجہ_t<غیرعلامتی> mutable_اشاریہ_for_انحراف (غیرعلامتی گرہ_idx, ثابت خالی* انحراف)
  {
    TRY_تفویض (غیرعلامتی بچہ_idx, اشاریہ_for_انحراف (گرہ_idx, انحراف));
    اگر (unlikely (بچہ_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    خودکار& بچہ = vertices_[بچہ_idx];
    برائے (غیرعلامتی p : بچہ.parents_iter ())
    {
      اگر (p != گرہ_idx) {
        واپس duplicate (گرہ_idx, بچہ_idx, سچ);
      }
    }

    واپس Ok(بچہ_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  گراف_نتیجہ_t<منطقی> تفویض_spaces ()
  {
    TRY (update_parents ());

    برائے (خودکار& راس : vertices_.writer ())
    {
      راس.space = 0;
      راس.ترجیح = 0;
    }
    عدد_roots_for_space.resize (1);
    TRY (گراف_نتیجہ_t<خالی>::from (عدد_roots_for_space, تخصیص_FAILURE));
    عدد_roots_for_space[0] = 1;
    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    hb_سیٹ_t دیکھا;
    hb_سیٹ_t roots;
    TRY (ڈھونڈو_space_roots (دیکھا, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    دیکھا.invert ();

    اگر (!roots) واپس Ok(جھوٹ);

    جب (roots)
    {
      uint32_t next = HB_سیٹ_قدر_INVALID;
      TRY (گراف_نتیجہ_t<خالی>::from (roots, تخصیص_FAILURE));
      اگر (!roots.next (&next)) توڑو;

      hb_سیٹ_t connected_roots;
      TRY (ڈھونڈو_connected_nodes (next, roots, دیکھا, connected_roots));
      TRY (گراف_نتیجہ_t<خالی>::from (connected_roots, تخصیص_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (گراف_نتیجہ_t<خالی>::from (connected_roots, تخصیص_FAILURE));

      غیرعلامتی اگلا_space = یہ->اگلا_space ();
      اگر (unlikely (اگلا_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 اگلا_space);
        واپس Err(حد_بندی_EXCEEDED);
      }
      عدد_roots_for_space.push (0);
      TRY(گراف_نتیجہ_t<خالی>::from (عدد_roots_for_space, تخصیص_FAILURE));

      برائے (غیرعلامتی جڑ : connected_roots)
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Subgraph %u gets space %u", جڑ, اگلا_space);
        vertices_[جڑ].space = اگلا_space;
        عدد_roots_for_space[اگلا_space] = عدد_roots_for_space[اگلا_space] + 1;
        فاصلہ_invalid = سچ;
        positions_invalid = سچ;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    واپس Ok(سچ);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  گراف_نتیجہ_t<منطقی> isolate_subgraph (hb_سیٹ_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_سیٹ_t parents;
    برائے (غیرعلامتی جڑ_idx : roots)
    {
      اگر (unlikely (جڑ_idx >= vertices_.لمبائی))
        واپس Err(برآمد_کا_BOUNDS);
      subgraph.سیٹ (جڑ_idx, wide_parents (جڑ_idx, parents));
    }
    برائے (غیرعلامتی جڑ_idx : roots)
    {
      TRY (ڈھونڈو_subgraph (جڑ_idx, subgraph));
    }
    TRY (گراف_نتیجہ_t<خالی>::from (subgraph, تخصیص_FAILURE));

    hb_map_t اشاریہ_map;
    منطقی made_changes = جھوٹ;
    برائے (خودکار اندراج_2 : subgraph.iter ())
    {
      أكد (اندراج_2.پہلا < vertices_.لمبائی);
      ثابت خودکار& node = vertices_[اندراج_2.پہلا];
      غیرعلامتی subgraph_incoming_کنارے = اندراج_2.second;

      اگر (subgraph_incoming_کنارے < node.incoming_کنارے_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = سچ;
        TRY (duplicate_subgraph (اندراج_2.پہلا, اشاریہ_map));
      }
    }

    TRY (گراف_نتیجہ_t<خالی>::from (اشاریہ_map, تخصیص_FAILURE));

    اگر (!made_changes)
      واپس Ok(جھوٹ);

    خودکار new_subgraph =
        + subgraph.کلیدیں ()
        | hb_map([&] (uint32_t گرہ_idx) {
          ثابت uint32_t *v;
          اگر (اشاریہ_map.has (گرہ_idx, &v)) واپس *v;
          واپس گرہ_idx;
        })
        ;

    TRY(remap_obj_indices (اشاریہ_map, new_subgraph));
    TRY(remap_obj_indices (اشاریہ_map, parents.iter (), سچ));

    // Update roots set with new indices as needed.
    برائے (خودکار next : roots)
    {
      ثابت uint32_t *v;
      اگر (اشاریہ_map.has (next, &v))
      {
        roots.del (next);
        roots.جوڑو (*v);
      }
    }

    واپس Ok(سچ);
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
  سانچہ <قسم_نام دورہ_کنارہ_Func>
  گراف_نتیجہ_t<خالی> traverse_directed_bfs (غیرعلامتی آغاز_idx, دورہ_کنارہ_Func&& دورہ_کنارہ)
  {
    اگر (unlikely (آغاز_idx >= vertices_.لمبائی))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      واپس Err(برآمد_کا_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (ordering_scratch_, تخصیص_FAILURE));

    اگر (!دورہ_کنارہ (HB_CODEPOINT_INVALID, خالی_اشاریہ, آغاز_idx, 0))
      واپس Ok();

    غیرعلامتی سر_2 = 0;
    غیرعلامتی دم = 0;
    خودکار& قطار = ordering_scratch_;
    قطار[دم++] = آغاز_idx;

    غیرعلامتی گہرائی = 0;
    جب (سر_2 < دم)
    {
      غیرعلامتی level_انتہا = دم;
      جب (سر_2 < level_انتہا)
      {
        غیرعلامتی گرہ_idx = قطار[سر_2++];
        ثابت خودکار& v = vertices_[گرہ_idx];

        غیرعلامتی عدد_real = v.obj().real_links.لمبائی;
        غیرعلامتی کل_links = عدد_real + v.obj().virtual_links.لمبائی;

        برائے (غیرعلامتی i = 0; i < کل_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت خودکار& v = vertices_[گرہ_idx];
          ثابت خودکار& link = (i < عدد_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - عدد_real];
          غیرعلامتی بچہ_idx = link.objidx;

          اگر (!دورہ_کنارہ (گرہ_idx, &link, بچہ_idx, گہرائی + 1))
            جاری;

          اگر (unlikely (دم >= قطار.لمبائی))
            واپس Err(چکر_DETECTED);

          قطار[دم++] = بچہ_idx;
        }
      }
      گہرائی++;
    }
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> ڈھونڈو_subgraph (غیرعلامتی گرہ_idx, hb_map_t& subgraph)
  {
    واپس traverse_directed_bfs (گرہ_idx, [&] (
      غیرعلامتی والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      غیرعلامتی بچہ,
      غیرعلامتی گہرائی) {
      اگر (گہرائی == 0) واپس سچ;
      hb_codepoint_t *گنتی;
      اگر (subgraph.has (بچہ, &گنتی))
      {
        (*گنتی)++;
        واپس جھوٹ;
      }
      subgraph.سیٹ (بچہ, 1);
      واپس سچ;
    });
  }

  گراف_نتیجہ_t<خالی> ڈھونڈو_subgraph (غیرعلامتی گرہ_idx, hb_سیٹ_t& subgraph)
  {
    واپس traverse_directed_bfs (گرہ_idx, [&] (
      غیرعلامتی والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      غیرعلامتی بچہ,
      غیرعلامتی گہرائی) {
      اگر (subgraph.has (بچہ)) واپس جھوٹ;
      subgraph.جوڑو (بچہ);
      واپس سچ;
    });
  }

  گراف_نتیجہ_t<size_t> ڈھونڈو_subgraph_سائز (غیرعلامتی گرہ_idx, hb_سیٹ_t& subgraph, غیرعلامتی max_گہرائی = HB_گراف_INVALID)
  {
    size_t سائز = 0;
    TRY (traverse_directed_bfs (گرہ_idx, [&] (
      غیرعلامتی والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      غیرعلامتی بچہ,
      غیرعلامتی گہرائی) {
      اگر (subgraph.has (بچہ)) واپس جھوٹ;
      subgraph.جوڑو (بچہ);

      ثابت خودکار& o = vertices_[بچہ].obj();
      سائز += o.دم - o.سر_2;
      واپس گہرائی < max_گہرائی;
    }));
    واپس Ok(سائز);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  گراف_نتیجہ_t<خالی> ڈھونڈو_32bit_roots (غیرعلامتی گرہ_idx, hb_سیٹ_t& ملا)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_سیٹ_t دیکھا;
    واپس traverse_directed_bfs (گرہ_idx, [&] (
      غیرعلامتی والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      غیرعلامتی بچہ,
      غیرعلامتی _) {

      اگر (link && ملا.has(والد))
        // Don't traverse from something that's already marked as a root.
        واپس جھوٹ;

      اگر (link && !link->is_signed && link->چوڑائی == 4)
      {
        ملا.جوڑو (link->objidx);
        دیکھا.جوڑو (link->objidx);
        واپس جھوٹ;
      }

      اگر (دیکھا.has (بچہ)) واپس جھوٹ;
      دیکھا.جوڑو (بچہ);
      واپس سچ;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  سانچہ<قسم_نام O>
  گراف_نتیجہ_t<غیرعلامتی> move_بچہ (غیرعلامتی old_والد_idx,
                                       ثابت O* old_انحراف,
                                       غیرعلامتی new_والد_idx,
                                       ثابت O* new_انحراف)
  {
    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    اگر (unlikely (old_والد_idx >= vertices_.لمبائی ||
                  new_والد_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    خودکار& old_v = vertices_[old_والد_idx];
    خودکار& new_v = vertices_[new_والد_idx];

    TRY_تفویض (غیرعلامتی بچہ_id, اشاریہ_for_انحراف (old_والد_idx,
                                                     old_انحراف));
    اگر (unlikely (بچہ_id >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    TRY(new_v.جوڑو_real_link(O::static_سائز, بچہ_id, (ثابت حرف*) new_انحراف - (ثابت حرف*) new_v.obj().سر_2));

    خودکار& بچہ = vertices_[بچہ_id];
    TRY(بچہ.جوڑو_والد (new_والد_idx, جھوٹ));

    old_v.remove_real_link_unordered (بچہ_id, old_انحراف);
    بچہ.remove_والد (old_والد_idx);

    واپس Ok(بچہ_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  سانچہ<قسم_نام O>
  گراف_نتیجہ_t<خالی> move_children (غیرعلامتی old_والد_idx,
                                      غیرعلامتی old_pos_آغاز,
                                      غیرعلامتی old_pos_انتہا,
                                      غیرعلامتی new_والد_idx,
                                      غیرعلامتی new_pos_آغاز)
  {
    اگر (unlikely (old_والد_idx >= vertices_.لمبائی ||
                  new_والد_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    فاصلہ_invalid = سچ;
    positions_invalid = سچ;

    خودکار& old_v = vertices_[old_والد_idx];
    خودکار& new_v = vertices_[new_والد_idx];

    hb_سمتیہ_t<hb_serialize_context_t::object_t::link_t> old_links;
    برائے (ثابت خودکار& l : old_v.obj().real_links)
    {
      اگر (l.position < old_pos_آغاز || l.position >= old_pos_انتہا)
      {
        old_links.push (l);
        جاری;
      }

      غیرعلامتی array_pos = l.position - old_pos_آغاز;

      غیرعلامتی بچہ_id = l.objidx;
      اگر (unlikely (بچہ_id >= vertices_.لمبائی))
        واپس Err(برآمد_کا_BOUNDS);

      TRY(new_v.جوڑو_real_link(O::static_سائز, بچہ_id, new_pos_آغاز + array_pos));

      خودکار& بچہ = vertices_[بچہ_id];
      TRY(بچہ.جوڑو_والد (new_والد_idx, جھوٹ));
      بچہ.remove_والد (old_والد_idx);
    }

    TRY (گراف_نتیجہ_t<خالی>::from (old_links, تخصیص_FAILURE));
    واپس old_v.سیٹ_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  گراف_نتیجہ_t<خالی> duplicate_subgraph (غیرعلامتی گرہ_idx, hb_map_t& اشاریہ_map)
  {
    گراف_نتیجہ_t<خالی> نتیجہ = Ok();
    TRY (traverse_directed_bfs (گرہ_idx, [&] (
      غیرعلامتی والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      غیرعلامتی بچہ,
      غیرعلامتی _) {
      اگر (نتیجہ.is_err () || اشاریہ_map.has (بچہ)) واپس جھوٹ;
      خودکار r = duplicate (بچہ);
      اگر (unlikely (r.is_err ()))
      {
        نتیجہ = Err(r.error ());
        واپس جھوٹ;
      }
      اشاریہ_map.سیٹ (بچہ, *r);
      واپس سچ;
    }));

    واپس گراف_نتیجہ_t<خالی>::from(اشاریہ_map, تخصیص_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  گراف_نتیجہ_t<غیرعلامتی> duplicate (غیرعلامتی گرہ_idx, منطقی copy_جدول = جھوٹ)
  {
    اگر (unlikely (گرہ_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    اگر (unlikely (vertices_.لمبائی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.لمبائی);
      واپس Err(حد_بندی_EXCEEDED);
    }

    positions_invalid = سچ;
    فاصلہ_invalid = سچ;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجہ_t<خالی>::from (vertices_, تخصیص_FAILURE));

    غیرعلامتی clone_idx = vertices_.لمبائی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجہ_t<خالی>::from (ordering_, تخصیص_FAILURE));

    خودکار& بچہ = vertices_[گرہ_idx];

    غیرعلامتی جدول_سائز = بچہ.obj().دم - بچہ.obj().سر_2;
    اگر (copy_جدول && جدول_سائز)
    {
      حرف* بفر = (حرف*) hb_malloc (جدول_سائز);
      اگر (unlikely (!بفر))
        واپس Err(تخصیص_FAILURE);
      خودکار res = جوڑو_بفر (بفر);
      اگر (unlikely (res.is_err ()))
      {
        hb_free (بفر);
        واپس Err(res.error ());
      }
      hb_memcpy (بفر, بچہ.obj().سر_2, جدول_سائز);
      clone->سیٹ_بفر(بفر, بفر + جدول_سائز);
    }
    ورنہ
    {
      clone->سیٹ_بفر(بچہ.obj().سر_2, بچہ.obj().دم);
    }
    clone->distance = بچہ.distance;
    clone->space = بچہ.space;
    clone->reset_parents ();

    برائے (ثابت خودکار& l : بچہ.obj().real_links)
    {
      TRY(clone->جوڑو_real_link (l));
      TRY(vertices_[l.objidx].جوڑو_والد (clone_idx, جھوٹ));
    }

    برائے (ثابت خودکار& l : بچہ.obj().virtual_links)
    {
      TRY(clone->جوڑو_virtual_link (l));
      TRY(vertices_[l.objidx].جوڑو_والد (clone_idx, سچ));
    }

    واپس Ok(clone_idx);
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
  گراف_نتیجہ_t<غیرعلامتی> duplicate (غیرعلامتی والد_idx, غیرعلامتی بچہ_idx, منطقی copy_جدول = جھوٹ)
  {
    اگر (unlikely (والد_idx >= vertices_.لمبائی || بچہ_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& بچہ = vertices_[بچہ_idx];
    غیرعلامتی links_مقصد_بچہ = بچہ.incoming_کنارے_from_والد (والد_idx);

    اگر (unlikely (بچہ.incoming_کنارے_2 () <= links_مقصد_بچہ || بچہ.has_incoming_virtual_کنارے_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  Not duplicating %u => %u",
                 والد_idx, بچہ_idx);
      واپس Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  Duplicating %u => %u",
               والد_idx, بچہ_idx);

    TRY_تفویض (غیرعلامتی clone_idx, duplicate (بچہ_idx, copy_جدول));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    اگر (والد_idx == clone_idx) والد_idx++;

    خودکار& والد = vertices_[والد_idx];
    غیرعلامتی گنتی = 0;
    غیرعلامتی عدد_real = والد.obj().real_links.لمبائی;
    برائے (خودکار& l : والد.all_links_writer ())
    {
      گنتی++;
      اگر (l.objidx != بچہ_idx)
        جاری;

      TRY(reassign_link (l, والد_idx, clone_idx, گنتی > عدد_real));
    }

    واپس Ok(clone_idx);
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
  گراف_نتیجہ_t<غیرعلامتی> duplicate (ثابت hb_سیٹ_t* parents, غیرعلامتی بچہ_idx)
  {
    اگر (unlikely (!parents || parents->is_empty ()))
      واپس Err(INVALID_دلیل);

    اگر (unlikely (بچہ_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& بچہ = vertices_[بچہ_idx];
    غیرعلامتی links_مقصد_بچہ = 0;
    غیرعلامتی آخری_والد = parents->get_max ();
    غیرعلامتی پہلا_والد = parents->get_min ();
    برائے (غیرعلامتی والد_idx : *parents) {
      اگر (unlikely (والد_idx >= vertices_.لمبائی))
        واپس Err(برآمد_کا_BOUNDS);
      links_مقصد_بچہ += بچہ.incoming_کنارے_from_والد (والد_idx);
    }

    اگر (unlikely (بچہ.incoming_کنارے_2 () <= links_مقصد_بچہ || بچہ.has_incoming_virtual_کنارے_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  Not duplicating %u, ..., %u => %u", پہلا_والد, آخری_والد, بچہ_idx);
      واپس Err(INVALID_دلیل);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  Duplicating %u, ..., %u => %u", پہلا_والد, آخری_والد, بچہ_idx);

    TRY_تفویض (غیرعلامتی clone_idx, duplicate (بچہ_idx));

    برائے (غیرعلامتی والد_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      اگر (والد_idx == clone_idx) والد_idx++;
      خودکار& والد = vertices_[والد_idx];
      غیرعلامتی گنتی = 0;
      غیرعلامتی عدد_real = والد.obj().real_links.لمبائی;
      برائے (خودکار& l : والد.all_links_writer ())
      {
        گنتی++;
        اگر (l.objidx != بچہ_idx)
          جاری;

        TRY(reassign_link (l, والد_idx, clone_idx, گنتی > عدد_real));
      }
    }

    واپس Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  گراف_نتیجہ_t<غیرعلامتی> new_گرہ (حرف* سر_2, حرف* دم)
  {
    اگر (unlikely (vertices_.لمبائی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.لمبائی);
      واپس Err(حد_بندی_EXCEEDED);
    }

    positions_invalid = سچ;
    فاصلہ_invalid = سچ;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجہ_t<خالی>::from (vertices_, تخصیص_FAILURE));

    غیرعلامتی clone_idx = vertices_.لمبائی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجہ_t<خالی>::from (ordering_, تخصیص_FAILURE));

    clone->سیٹ_بفر (سر_2, دم);
    clone->distance = 0;
    clone->space = 0;

    واپس Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  گراف_نتیجہ_t<غیرعلامتی> remap_بچہ (غیرعلامتی والد_idx, غیرعلامتی old_بچہ_idx)
  {
    اگر (unlikely (والد_idx >= vertices_.لمبائی || old_بچہ_idx >= vertices_.لمبائی))
      واپس Err(برآمد_کا_BOUNDS);

    TRY_تفویض (غیرعلامتی new_بچہ_idx, duplicate (old_بچہ_idx));

    خودکار& والد = vertices_[والد_idx];
    برائے (خودکار& l : والد.real_links_writer ())
    {
      اگر (l.objidx != old_بچہ_idx)
        جاری;
      TRY(reassign_link (l, والد_idx, new_بچہ_idx, جھوٹ));
    }

    برائے (خودکار& l : والد.virtual_links_writer ())
    {
      اگر (l.objidx != old_بچہ_idx)
        جاری;
      TRY(reassign_link (l, والد_idx, new_بچہ_idx, سچ));
    }
    واپس Ok(new_بچہ_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  گراف_نتیجہ_t<منطقی> raise_childrens_ترجیح (غیرعلامتی والد_idx)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  Raising priority of all children of %u",
               والد_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    اگر (unlikely (والد_idx >= vertices_.لمبائی)) واپس Err(برآمد_کا_BOUNDS);
    منطقی made_change = جھوٹ;
    برائے (خودکار& l : vertices_[والد_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_ترجیح ();
    واپس Ok(made_change);
  }

  گراف_نتیجہ_t<خالی> is_fully_connected ()
  {
    TRY(update_parents());

    اگر (unlikely (جڑ().incoming_کنارے_2 ()))
      // Root cannot have parents.
      واپس Err(ORPHANED_NODES);

    برائے (غیرعلامتی i = 0; i < جڑ_idx (); i++)
    {
      اگر (unlikely (!vertices_[i].incoming_کنارے_2 ()))
        واپس Err(ORPHANED_NODES);
    }
    واپس Ok();
  }

#اگر 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  خالی save_fuzzer_بیج (hb_لیبل_t لیبل_2) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((خالی*) &لیبل_2, حجم (لیبل_2), 1, f);

    uint16_t عدد_objects = vertices_.لمبائی;
    fwrite ((خالی*) &عدد_objects, حجم (عدد_objects), 1, f);

    برائے (ثابت خودکار& v : vertices_)
    {
      uint16_t blob_سائز = v.جدول_سائز ();
      fwrite ((خالی*) &blob_سائز, حجم (blob_سائز), 1, f);
      fwrite ((ثابت خالی*) v.obj.سر_2, blob_سائز, 1, f);
    }

    uint16_t link_گنتی = 0;
    برائے (ثابت خودکار& v : vertices_)
      link_گنتی += v.obj.real_links.لمبائی;

    fwrite ((خالی*) &link_گنتی, حجم (link_گنتی), 1, f);

    قسمتعریف ساخت
    {
      uint16_t والد;
      uint16_t بچہ;
      uint16_t position;
      uint8_t چوڑائی;
    } link_t;

    برائے (غیرعلامتی i = 0; i < vertices_.لمبائی; i++)
    {
      برائے (ثابت خودکار& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.چوڑائی
        };
        fwrite ((خالی*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#اگر_ختم

  غیرعلامتی عدد_roots_for_space_2 (غیرعلامتی space) ثابت
  {
    واپس عدد_roots_for_space[space];
  }

  غیرعلامتی اگلا_space () ثابت
  {
    واپس عدد_roots_for_space.لمبائی;
  }

  گراف_نتیجہ_t<خالی> move_مقصد_new_space (ثابت hb_سیٹ_t& indices)
  {
    اگر (unlikely (عدد_roots_for_space.لمبائی >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 عدد_roots_for_space.لمبائی);
      واپس Err(حد_بندی_EXCEEDED);
    }
    عدد_roots_for_space.push (0);
    TRY (گراف_نتیجہ_t<خالی>::from (عدد_roots_for_space, تخصیص_FAILURE));
    غیرعلامتی new_space = عدد_roots_for_space.لمبائی - 1;

    برائے (غیرعلامتی اشاریہ_2 : indices) {
      اگر (unlikely (اشاریہ_2 >= vertices_.لمبائی))
        واپس Err(برآمد_کا_BOUNDS);
      خودکار& node = vertices_[اشاریہ_2];
      عدد_roots_for_space[node.space] = عدد_roots_for_space[node.space] - 1;
      عدد_roots_for_space[new_space] = عدد_roots_for_space[new_space] + 1;
      node.space = new_space;
      فاصلہ_invalid = سچ;
      positions_invalid = سچ;
    }
    واپس Ok();
  }

  غیرعلامتی space_for (غیرعلامتی اشاریہ_2, غیرعلامتی* جڑ = خالی_اشاریہ) ثابت
  {
    جب (سچ)
    {
      أكد (اشاریہ_2 < vertices_.لمبائی);
      ثابت خودکار& node = vertices_[اشاریہ_2];
      اگر (node.space)
      {
        اگر (جڑ != خالی_اشاریہ)
          *جڑ = اشاریہ_2;
        واپس node.space;
      }

      اگر (!node.incoming_کنارے_2 ())
      {
        اگر (جڑ)
          *جڑ = اشاریہ_2;
        واپس 0;
      }

      اشاریہ_2 = *node.parents_iter ();
    }
  }

  size_t کل_سائز_اندر_بائٹیں () ثابت {
    size_t کل_سائز = 0;
    غیرعلامتی گنتی = vertices_.لمبائی;
    برائے (غیرعلامتی i = 0; i < گنتی; i++) {
      size_t سائز = vertices_.arrayZ[i].جدول_سائز ();
      کل_سائز += سائز;
    }
    واپس کل_سائز;
  }


 نجی:

  خالی print_orphaned_nodes ()
  {
    اگر (!DEBUG_ENABLED(SUBSET_REPACK)) واپس;

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Graph is not fully connected.");

    parents_invalid = سچ;
    (خالی) update_parents();

    اگر (جڑ().incoming_کنارے_2 ()) {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Root node has incoming edges.");
    }

    برائے (غیرعلامتی i = 0; i < vertices_.لمبائی; i++)
    {
      ثابت خودکار& v = vertices_[i];
      اگر (!v.incoming_کنارے_2 ())
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  غیرعلامتی wide_parents (غیرعلامتی گرہ_idx, hb_سیٹ_t& parents) ثابت
  {
    غیرعلامتی گنتی = 0;
    برائے (غیرعلامتی p : vertices_[گرہ_idx].parents_iter ())
    {
      // Only real links can be wide
      برائے (ثابت خودکار& l : vertices_[p].obj().real_links)
      {
        اگر (l.objidx == گرہ_idx
            && (l.چوڑائی == 3 || l.چوڑائی == 4)
            && !l.is_signed)
        {
          گنتی++;
          parents.جوڑو (p);
        }
      }
    }
    واپس گنتی;
  }

 عوامی:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  گراف_نتیجہ_t<خالی> update_parents ()
  {
    اگر (!parents_invalid) واپس Ok();

    غیرعلامتی گنتی = vertices_.لمبائی;

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      vertices_.arrayZ[i].reset_parents ();

    برائے (غیرعلامتی p = 0; p < گنتی; p++)
    {
      برائے (ثابت خودکار& l : vertices_.arrayZ[p].obj().real_links)
      {
        اگر (unlikely (l.objidx >= گنتی)) واپس Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جوڑو_والد (p, جھوٹ));
      }

      برائے (ثابت خودکار& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        اگر (unlikely (l.objidx >= گنتی)) واپس Err(INVALID_دلیل);
        TRY(vertices_[l.objidx].جوڑو_والد (p, سچ));
      }
    }

    parents_invalid = جھوٹ;
    واپس Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  خالی update_positions ()
  {
    اگر (!positions_invalid) واپس;

    غیرعلامتی current_pos = 0;
    برائے (غیرعلامتی i : ordering_)
    {
      خودکار& v = vertices_[i];
      v.آغاز = current_pos;
      current_pos += v.obj().دم - v.obj().سر_2;
      v.انتہا = current_pos;
    }

    positions_invalid = جھوٹ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  گراف_نتیجہ_t<خالی> update_distances ()
  {
    اگر (!فاصلہ_invalid) واپس Ok();

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
    غیرعلامتی گنتی = vertices_.لمبائی;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[جڑ_idx ()].distance = 0;

    hb_ترجیح_قطار_t<int64_t> قطار;
    قطار.alloc (گنتی);
    قطار.insert (0, جڑ_idx ());
    TRY (گراف_نتیجہ_t<خالی>::from (قطار, تخصیص_FAILURE));

    hb_سمتیہ_t<منطقی> دیکھا;
    دیکھا.resize (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (دیکھا, تخصیص_FAILURE));

    جب (!قطار.اندر_error () && !قطار.is_empty ())
    {
      غیرعلامتی اگلا_idx = قطار.نکالو_minimum ().second;
      اگر (دیکھا[اگلا_idx]) جاری;
      ثابت خودکار& next = vertices_[اگلا_idx];
      int64_t اگلا_فاصلہ = next.distance;
      دیکھا[اگلا_idx] = سچ;

      برائے (ثابت خودکار& link : next.obj().all_links ())
      {
        اگر (unlikely (link.objidx >= vertices_.لمبائی))
          واپس Err(برآمد_کا_BOUNDS);

        اگر (دیکھا[link.objidx]) جاری;

        خودکار& بچہ_v = vertices_.arrayZ[link.objidx];
        ثابت خودکار& بچہ = بچہ_v.obj ();
        غیرعلامتی link_چوڑائی = link.چوڑائی ? link.چوڑائی : 4; // treat virtual offsets as 32 bits wide
        int64_t بچہ_وزن = (بچہ.دم - بچہ.سر_2) +
                               ((int64_t) 1 << (link_چوڑائی * 8)) * (بچہ_v.space + 1);
        int64_t بچہ_فاصلہ = اگلا_فاصلہ + بچہ_وزن;

        اگر (بچہ_فاصلہ < بچہ_v.distance)
        {
          بچہ_v.distance = بچہ_فاصلہ;
          قطار.insert (بچہ_فاصلہ, link.objidx);
        }
      }
    }

    TRY (گراف_نتیجہ_t<خالی>::from (قطار, تخصیص_FAILURE));
    اگر (unlikely (!قطار.is_empty ()))
    {
      print_orphaned_nodes ();
      واپس Err(ORPHANED_NODES);
    }

    فاصلہ_invalid = جھوٹ;
    واپس Ok();
  }

 نجی:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  گراف_نتیجہ_t<خالی> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      غیرعلامتی والد_idx,
                      غیرعلامتی new_idx,
                      منطقی is_virtual)
  {
    غیرعلامتی old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_والد (والد_idx);
    واپس vertices_[new_idx].جوڑو_والد (والد_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  سانچہ<قسم_نام Iterator, hb_requires (hb_is_iterator (Iterator))>
  گراف_نتیجہ_t<خالی> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقی only_wide = جھوٹ)
  {
    اگر (!id_map) واپس Ok();
    برائے (غیرعلامتی i : subgraph)
    {
      خودکار& راس = vertices_[i];
      غیرعلامتی عدد_real = راس.obj().real_links.لمبائی;
      غیرعلامتی گنتی = 0;
      برائے (خودکار& link : راس.all_links_writer ())
      {
        گنتی++;
        ثابت uint32_t *v;
        اگر (!id_map.has (link.objidx, &v)) جاری;
        اگر (only_wide && (link.is_signed || (link.چوڑائی != 4 && link.چوڑائی != 3))) جاری;

        TRY(reassign_link (link, i, *v, گنتی > عدد_real));
      }
    }
    واپس Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  گراف_نتیجہ_t<خالی> ڈھونڈو_connected_nodes (غیرعلامتی آغاز_idx,
                                             hb_سیٹ_t& targets,
                                             hb_سیٹ_t& دیکھا,
                                             hb_سیٹ_t& connected)
  {
    TRY (گراف_نتیجہ_t<خالی>::from (دیکھا, تخصیص_FAILURE));
    اگر (دیکھا.has (آغاز_idx)) واپس Ok();
    اگر (unlikely (آغاز_idx >= vertices_.لمبائی))
    {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      واپس Err(برآمد_کا_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.لمبائی);
    TRY (گراف_نتیجہ_t<خالی>::from (ordering_scratch_, تخصیص_FAILURE));

    خودکار& ڈھیر = ordering_scratch_;
    غیرعلامتی ڈھیر_len = 0;

    گراف_نتیجہ_t<خالی> res = Ok();
    خودکار handle_گرہ = [&] (غیرعلامتی گرہ_idx) {
      دیکھا.جوڑو (گرہ_idx);
      اگر (targets.has (گرہ_idx)) {
        targets.del (گرہ_idx);
        connected.جوڑو (گرہ_idx);
      }

      اگر (unlikely (ڈھیر_len >= ڈھیر.لمبائی))
      {
        res = Err(چکر_DETECTED);
        واپس جھوٹ;
      }
      ڈھیر[ڈھیر_len++] = گرہ_idx;
      واپس سچ;
    };

    اگر (!handle_گرہ (آغاز_idx)) واپس res;

    جب (ڈھیر_len > 0)
    {
      غیرعلامتی گرہ_idx = ڈھیر[--ڈھیر_len];
      ثابت خودکار& v = vertices_[گرہ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      برائے (ثابت خودکار& l : v.obj().all_links ())
      {
        غیرعلامتی بچہ_idx = l.objidx;
        اگر (دیکھا.has (بچہ_idx)) جاری;
        اگر (!handle_گرہ (بچہ_idx)) واپس res;
      }

      برائے (غیرعلامتی والد_idx : v.parents_iter ())
      {
        اگر (دیکھا.has (والد_idx)) جاری;
        اگر (!handle_گرہ (والد_idx)) واپس res;
      }
    }

    TRY (گراف_نتیجہ_t<خالی>::from (دیکھا, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (connected, تخصیص_FAILURE));

    واپس Ok();
  }

 عوامی:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_سمتیہ_t<راس_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_سمتیہ_t<غیرعلامتی> ordering_;
  hb_سمتیہ_t<غیرعلامتی> ordering_scratch_;

 نجی:
  منطقی parents_invalid = سچ;
  منطقی فاصلہ_invalid = سچ;
  منطقی positions_invalid = سچ;
  hb_سمتیہ_t<غیرعلامتی> عدد_roots_for_space;
  hb_سمتیہ_t<حرف*> buffers;
};

}

#اگر_ختم  // GRAPH_GRAPH_HH
