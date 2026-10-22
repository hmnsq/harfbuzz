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

#ضمن "graph.hh"
#ضمن "../hb-ot-layout-gsubgpos.hh"
#ضمن "../OT/Layout/GSUB/ExtensionSubst.hh"
#ضمن "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#ضمن "gsubgpos-context.hh"
#ضمن "pairpos-graph.hh"
#ضمن "markbasepos-graph.hh"
#ضمن "ligature-graph.hh"

#جيڪڏهن_وصف_نه_ٿيل گراف_GSUBGPOS_گراف_HH
#وصف_ڪريو گراف_GSUBGPOS_گراف_HH

نطاق گراف {

بنية Lookup;

سانچو<قسم_نالو T>
بنية ExtensionFormat1 : عام OT::ExtensionFormat1<T>
{
  فراغ reset(بنانشان type)
  {
    هذا->format = 1;
    هذا->extensionLookupType = type;
    هذا->extension_وٿي = 0;
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < OT::ExtensionFormat1<T>::static_سائيز))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  بنانشان get_lookup_type () ثابت
  {
    أرجع هذا->extensionLookupType;
  }

  گراف_نتيجو_t<بنانشان> get_subtable_اشاريو (گراف_t& گراف, بنانشان this_اشاريو) ثابت
  {
    أرجع گراف.اشاريو_for_وٿي (this_اشاريو, &هذا->extension_وٿي);
  }
};

بنية Lookup : عام OT::Lookup
{
  بنانشان عدد_جو_subtables () ثابت
  {
    أرجع sub_جدول.len;
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < OT::Lookup::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < هذا->get_سائيز ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  منطقي is_extension (hb_ٽيگ_t جدول_ٽيگ_2) ثابت
  {
    أرجع lookupType == extension_type (جدول_ٽيگ_2);
  }

  منطقي use_mark_filtering_سيٽ () ثابت
  {
    بنانشان flag = lookupFlag;
    أرجع flag & 0x0010u;
  }

  گراف_نتيجو_t<فراغ> make_extension (gsubgpos_گراف_context_t& c,
                                       بنانشان this_اشاريو)
  {
    بنانشان type = lookupType;
    بنانشان ext_type = extension_type (c.جدول_ٽيگ_2);
    إذا (!ext_type || is_extension (c.جدول_ٽيگ_2))
    {
      // NOOP
      أرجع Ok();
    }

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_اشاريو);

    hb_سيٽ_t ڏٺل;
    hb_ويڪٽر_t<بنانشان> subtables;
    لكل (بنانشان i = 0; i < sub_جدول.len; i++)
    {
      TRY_سونپيو (بنانشان subtable_اشاريو, c.گراف.اشاريو_for_وٿي (this_اشاريو, &sub_جدول[i]));
      إذا (ڏٺل.has (subtable_اشاريو))
        استمر;
      ڏٺل.جوڙيو (subtable_اشاريو);
      subtables.push (subtable_اشاريو);
    }
    TRY (گراف_نتيجو_t<فراغ>::from (ڏٺل, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (subtables, ورهاست_FAILURE));

    لكل (بنانشان subtable_اشاريو : subtables)
      TRY (make_subtable_extension (c, this_اشاريو, subtable_اشاريو));

    lookupType = ext_type;
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> ورهايو_subtables_if_needed (gsubgpos_گراف_context_t& c,
                                                  بنانشان this_اشاريو)
  {
    بنانشان type = lookupType;
    منطقي is_ext = is_extension (c.جدول_ٽيگ_2);

    إذا (c.جدول_ٽيگ_2 != HB_OT_ٽيگ_GPOS && c.جدول_ٽيگ_2 != HB_OT_ٽيگ_GSUB)
      أرجع Ok();

    إذا (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      أرجع Ok();

    hb_ويڪٽر_t<hb_pair_t<بنانشان, hb_ويڪٽر_t<بنانشان>>> all_new_subtables;
    لكل (بنانشان i = 0; i < sub_جدول.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      خودڪار idx_res = c.گراف.اشاريو_for_وٿي (this_اشاريو, &sub_جدول[i]);
      إذا (!idx_res.is_ok ()) استمر;
      بنانشان subtable_اشاريو = *idx_res;
      إذا (is_ext) {
        بنانشان ext_subtable_اشاريو = subtable_اشاريو;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.گراف.object (ext_subtable_اشاريو).سر_2;
        إذا (!extension || !extension->sanitize (c.گراف.vertices_[ext_subtable_اشاريو]).is_ok ())
          استمر;

        TRY_سونپيو (subtable_اشاريو, extension->get_subtable_اشاريو (c.گراف, ext_subtable_اشاريو));
        type = extension->get_lookup_type ();
        إذا (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          استمر;
      }

      hb_ويڪٽر_t<بنانشان>* ورهايو_نتيجو;
      إذا (c.ورهايو_subtables.has (subtable_اشاريو, &ورهايو_نتيجو))
      {
        إذا (ورهايو_نتيجو->ڊيگهائي == 0)
          استمر;
        all_new_subtables.push (hb_pair(i, *ورهايو_نتيجو));
        TRY (گراف_نتيجو_t<فراغ>::from (all_new_subtables, ورهاست_FAILURE));
      }
      وإلا
      {
        hb_ويڪٽر_t<بنانشان> new_sub_tables;

        إذا (c.جدول_ٽيگ_2 == HB_OT_ٽيگ_GPOS) {
          بدل (type)
          {
          حالة 2:
            {
              TRY_سونپيو (new_sub_tables, ورهايو_subtable<PairPos> (c, subtable_اشاريو));
              اكسر;
            }
          حالة 4:
            {
              TRY_سونپيو (new_sub_tables, ورهايو_subtable<Mark_بنياد_Pos> (c, subtable_اشاريو));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        } وإلا إذا (c.جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB) {
          بدل (type)
          {
          حالة 4:
            {
              TRY_سونپيو (new_sub_tables, ورهايو_subtable<گراف::LigatureSubst> (c, subtable_اشاريو));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        }

        c.ورهايو_subtables.سيٽ (subtable_اشاريو, new_sub_tables);
        TRY (گراف_نتيجو_t<فراغ>::from (c.ورهايو_subtables, ورهاست_FAILURE));
        إذا (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (گراف_نتيجو_t<فراغ>::from (all_new_subtables, ورهاست_FAILURE));
        }
      }
    }

    إذا (all_new_subtables) {
      TRY (جوڙيو_sub_tables (c, this_اشاريو, type, all_new_subtables));
    }

    أرجع Ok();
  }

  سانچو<قسم_نالو T>
  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtable (gsubgpos_گراف_context_t& c,
                                                        بنانشان objidx)
  {
    T* sub_جدول_2 = (T*) c.گراف.object (objidx).سر_2;
    إذا (!sub_جدول_2 || !sub_جدول_2->sanitize (c.گراف.vertices_[objidx]).is_ok ())
      أرجع Ok(hb_ويڪٽر_t<بنانشان> ());

    أرجع sub_جدول_2->ورهايو_subtables (c, objidx);
  }

  گراف_نتيجو_t<فراغ> جوڙيو_sub_tables (gsubgpos_گراف_context_t& c,
                                       بنانشان this_اشاريو,
                                       بنانشان type,
                                       ثابت hb_ويڪٽر_t<hb_pair_t<بنانشان, hb_ويڪٽر_t<بنانشان>>>& subtable_ids)
  {
    منطقي is_ext = is_extension (c.جدول_ٽيگ_2);
    خودڪار* v = &c.گراف.vertices_[this_اشاريو];
    fix_existing_subtable_links (c, this_اشاريو, subtable_ids);

    بنانشان new_subtable_ڳڻپ = 0;
    لكل (ثابت خودڪار& p : subtable_ids)
      new_subtable_ڳڻپ += p.second.ڊيگهائي;

    size_t new_سائيز = v->جدول_سائيز ()
                      + new_subtable_ڳڻپ * OT::Offset16::static_سائيز;
    حرف* بفر = (حرف*) hb_calloc (1, new_سائيز);
    إذا (unlikely (!بفر)) أرجع Err(ورهاست_FAILURE);
    خودڪار res = c.جوڙيو_بفر (بفر);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      أرجع res;
    }
    hb_memcpy (بفر, v->obj ().سر_2, v->جدول_سائيز());

    إذا (use_mark_filtering_سيٽ ())
      hb_memcpy (بفر + new_سائيز - 2, v->obj ().پڇ - 2, 2);

    v->سيٽ_بفر (بفر, بفر + new_سائيز);

    Lookup* new_lookup = (Lookup*) بفر;

    بنانشان سرڪايو = 0;
    new_lookup->sub_جدول.len = sub_جدول.len + new_subtable_ڳڻپ;
    لكل (ثابت خودڪار& p : subtable_ids)
    {
      بنانشان وٿي_اشاريو = p.پهريون + سرڪايو + 1;
      سرڪايو += p.second.ڊيگهائي;

      لكل (بنانشان subtable_id : p.second)
      {
        إذا (is_ext)
        {
          TRY_سونپيو (بنانشان ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.گراف.vertices_[subtable_id].جوڙيو_والد (ext_id, خطأ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.گراف.vertices_[this_اشاريو];
        }

        TRY(v->جوڙيو_real_link(2, subtable_id,
          (حرف*) &new_lookup->sub_جدول[وٿي_اشاريو++] - (حرف*) new_lookup));
        TRY(c.گراف.vertices_[subtable_id].جوڙيو_والد (this_اشاريو, خطأ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->ترتيب_ڏيو_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.سيٽ (this_اشاريو, new_lookup);
    TRY (گراف_نتيجو_t<فراغ>::from (c.lookups, ورهاست_FAILURE));
    أرجع Ok();
  }

  فراغ fix_existing_subtable_links (gsubgpos_گراف_context_t& c,
                                    بنانشان this_اشاريو,
                                    ثابت hb_ويڪٽر_t<hb_pair_t<بنانشان, hb_ويڪٽر_t<بنانشان>>>& subtable_ids)
  {
    خودڪار& v = c.گراف.vertices_[this_اشاريو];
    بنانشان سرڪايو = 0;
    لكل (ثابت خودڪار& p : subtable_ids)
    {
      بنانشان وجهو_اشاريو = p.پهريون + سرڪايو;
      بنانشان pos_وٿي = p.second.ڊيگهائي * OT::Offset16::static_سائيز;
      بنانشان وجهو_وٿي = Lookup::min_سائيز + وجهو_اشاريو * OT::Offset16::static_سائيز;
      سرڪايو += p.second.ڊيگهائي;

      لكل (خودڪار& l : v.all_links_writer ())
      {
        إذا (l.position > وجهو_وٿي) l.position += pos_وٿي;
      }
    }
  }

  گراف_نتيجو_t<بنانشان> create_extension_subtable (gsubgpos_گراف_context_t& c,
                                                      بنانشان subtable_اشاريو,
                                                      بنانشان type)
  {
    بنانشان extension_سائيز = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_سائيز;

    TRY_سونپيو (بنانشان ext_اشاريو, c.create_ڳنڍ (extension_سائيز));

    خودڪار& ext_راس = c.گراف.vertices_[ext_اشاريو];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_راس.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_راس.جوڙيو_real_link (4, subtable_اشاريو, 4));

    أرجع Ok(ext_اشاريو);
  }

  گراف_نتيجو_t<فراغ> make_subtable_extension (gsubgpos_گراف_context_t& c,
                                                بنانشان lookup_اشاريو,
                                                بنانشان subtable_اشاريو)
  {
    بنانشان type = lookupType;
    بنانشان ext_اشاريو = HB_گراف_INVALID;
    بنانشان* existing_ext_اشاريو = خالي_اشارو;
    إذا (c.subtable_منزل_extension.has(subtable_اشاريو, &existing_ext_اشاريو)) {
      ext_اشاريو = *existing_ext_اشاريو;
    } وإلا {
      TRY_سونپيو (ext_اشاريو, create_extension_subtable(c, subtable_اشاريو, type));
      c.subtable_منزل_extension.سيٽ(subtable_اشاريو, ext_اشاريو);
      TRY (گراف_نتيجو_t<فراغ>::from (c.subtable_منزل_extension, ورهاست_FAILURE));
    }

    خودڪار& subtable_راس = c.گراف.vertices_[subtable_اشاريو];
    خودڪار& lookup_راس = c.گراف.vertices_[lookup_اشاريو];
    بنانشان replaced = 0;
    لكل (خودڪار& l : lookup_راس.real_links_writer ())
    {
      إذا (l.objidx == subtable_اشاريو) {
        // Change lookup to point at the extension.
        l.objidx = ext_اشاريو;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    خودڪار& ext_راس = c.گراف.vertices_[ext_اشاريو];
    لكل (بنانشان i = 0; i < replaced; i++)
    {
      subtable_راس.remove_والد (lookup_اشاريو);
      TRY(ext_راس.جوڙيو_والد (lookup_اشاريو, خطأ));
    }
    إذا (!existing_ext_اشاريو)
      TRY(subtable_راس.جوڙيو_والد (ext_اشاريو, خطأ));

    أرجع Ok();
  }

 خاص:
  منطقي is_supported_gsub_type(بنانشان type, gsubgpos_گراف_context_t& c) ثابت {
    أرجع (c.جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_جدول::Type::Ligature
    );
  }

  منطقي is_supported_gpos_type(بنانشان type, gsubgpos_گراف_context_t& c) ثابت {
   أرجع (c.جدول_ٽيگ_2 == HB_OT_ٽيگ_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Mark_بنياد
    );
  }

  بنانشان extension_type (hb_ٽيگ_t جدول_ٽيگ_2) ثابت
  {
    بدل (جدول_ٽيگ_2)
    {
    حالة HB_OT_ٽيگ_GPOS: أرجع 9;
    حالة HB_OT_ٽيگ_GSUB: أرجع 7;
    افتراضي: أرجع 0;
    }
  }
};

سانچو <قسم_نالو T>
بنية LookupList : عام OT::LookupList<T>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < OT::LookupList<T>::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < OT::LookupList<T>::شي_سائيز * هذا->len)) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية GSTAR : عام OT::GSUBGPOS
{
  ساكن گراف_نتيجو_t<ثابت GSTAR*> گراف_منزل_gstar (گراف_t& گراف)
  {
    ثابت خودڪار& r = گراف.پاڙ ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().سر_2;
    إذا (unlikely (!gstar)) أرجع Err(INVALID_دليل);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    أرجع gstar;
  }

  ثابت فراغ* get_lookup_list_ميدان_وٿي () ثابت
  {
    أرجع OT::GSUBGPOS::get_lookup_list_ميدان_وٿي ();
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t len = راس.جدول_سائيز ();
    إذا (unlikely (len < OT::GSUBGPOS::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (نسخو.major != 1)) أرجع Err(SANITIZE_FAILURE);
    إذا (unlikely (len < get_سائيز ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> ڳوليو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بنانشان, Lookup*>& lookups /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && lookupList2)
      أرجع ڳوليو_lookups<MediumTypes> (گراف, lookups);
#جيڪڏهن_پورو
    TRY(ڳوليو_lookups<SmallTypes> (گراف, lookups));
    أرجع Ok();
  }

  گراف_نتيجو_t<بنانشان> get_lookup_list_اشاريو (گراف_t& گراف) ثابت
  {
    ثابت فراغ* وٿي = get_lookup_list_ميدان_وٿي ();
    إذا (unlikely (!وٿي)) أرجع Err(INVALID_دليل);
    أرجع گراف.اشاريو_for_وٿي (گراف.پاڙ_idx (), وٿي);
  }

  سانچو<قسم_نالو Types>
  گراف_نتيجو_t<فراغ> ڳوليو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بنانشان, Lookup*>& lookups /*ٻاهريون*/) ثابت
  {
    TRY_سونپيو (بنانشان lookup_list_idx, get_lookup_list_اشاريو (گراف));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) گراف.object (lookup_list_idx).سر_2;
    إذا (unlikely (!lookupList)) أرجع Err(INVALID_دليل);
    TRY(lookupList->sanitize (گراف.vertices_[lookup_list_idx]));

    لكل (بنانشان i = 0; i < lookupList->len; i++)
    {
      TRY_سونپيو (بنانشان lookup_idx, گراف.اشاريو_for_وٿي (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) گراف.object (lookup_idx).سر_2;
      إذا (!lookup || !lookup->sanitize (گراف.vertices_[lookup_idx]).is_ok ()) استمر;
      lookups.سيٽ (lookup_idx, lookup);
    }
    أرجع Ok();
  }
};




}

#جيڪڏهن_پورو  /* GRAPH_GSUBGPOS_GRAPH_HH */
