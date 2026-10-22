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

#که_نه_تعریف_شوی ګراف_GSUBGPOS_ګراف_HH
#تعریف_کړه ګراف_GSUBGPOS_ګراف_HH

نطاق ګراف {

بنية Lookup;

کينډۍ<ډول_نوم T>
بنية ExtensionFormat1 : عام OT::ExtensionFormat1<T>
{
  فراغ reset(بینښه type)
  {
    هذا->format = 1;
    هذا->extensionLookupType = type;
    هذا->extension_دننه_واټن = 0;
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < OT::ExtensionFormat1<T>::static_اندازه))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  بینښه get_lookup_type () ثابت
  {
    أرجع هذا->extensionLookupType;
  }

  ګراف_پایله_t<بینښه> get_subtable_شاخص (ګراف_t& ګراف, بینښه this_شاخص) ثابت
  {
    أرجع ګراف.شاخص_for_دننه_واټن (this_شاخص, &هذا->extension_دننه_واټن);
  }
};

بنية Lookup : عام OT::Lookup
{
  بینښه شمېره_د_subtables () ثابت
  {
    أرجع sub_جدول.len;
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < OT::Lookup::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < هذا->get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  منطقي is_extension (hb_نښان_t جدول_نښان_2) ثابت
  {
    أرجع lookupType == extension_type (جدول_نښان_2);
  }

  منطقي use_mark_filtering_سېټ () ثابت
  {
    بینښه flag = lookupFlag;
    أرجع flag & 0x0010u;
  }

  ګراف_پایله_t<فراغ> make_extension (gsubgpos_ګراف_context_t& c,
                                       بینښه this_شاخص)
  {
    بینښه type = lookupType;
    بینښه ext_type = extension_type (c.جدول_نښان_2);
    إذا (!ext_type || is_extension (c.جدول_نښان_2))
    {
      // NOOP
      أرجع Ok();
    }

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_شاخص);

    hb_سېټ_t کتل_شوی;
    hb_ویکټور_t<بینښه> subtables;
    لكل (بینښه i = 0; i < sub_جدول.len; i++)
    {
      TRY_ټاکل (بینښه subtable_شاخص, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &sub_جدول[i]));
      إذا (کتل_شوی.has (subtable_شاخص))
        استمر;
      کتل_شوی.جمع_کړه (subtable_شاخص);
      subtables.push (subtable_شاخص);
    }
    TRY (ګراف_پایله_t<فراغ>::from (کتل_شوی, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (subtables, ویش_FAILURE));

    لكل (بینښه subtable_شاخص : subtables)
      TRY (make_subtable_extension (c, this_شاخص, subtable_شاخص));

    lookupType = ext_type;
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> وېشل_subtables_if_needed (gsubgpos_ګراف_context_t& c,
                                                  بینښه this_شاخص)
  {
    بینښه type = lookupType;
    منطقي is_ext = is_extension (c.جدول_نښان_2);

    إذا (c.جدول_نښان_2 != HB_OT_نښان_GPOS && c.جدول_نښان_2 != HB_OT_نښان_GSUB)
      أرجع Ok();

    إذا (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      أرجع Ok();

    hb_ویکټور_t<hb_pair_t<بینښه, hb_ویکټور_t<بینښه>>> all_new_subtables;
    لكل (بینښه i = 0; i < sub_جدول.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      خپلکاری idx_res = c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &sub_جدول[i]);
      إذا (!idx_res.is_ok ()) استمر;
      بینښه subtable_شاخص = *idx_res;
      إذا (is_ext) {
        بینښه ext_subtable_شاخص = subtable_شاخص;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.ګراف.object (ext_subtable_شاخص).سر_2;
        إذا (!extension || !extension->sanitize (c.ګراف.vertices_[ext_subtable_شاخص]).is_ok ())
          استمر;

        TRY_ټاکل (subtable_شاخص, extension->get_subtable_شاخص (c.ګراف, ext_subtable_شاخص));
        type = extension->get_lookup_type ();
        إذا (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          استمر;
      }

      hb_ویکټور_t<بینښه>* وېشل_پایله;
      إذا (c.وېشل_subtables.has (subtable_شاخص, &وېشل_پایله))
      {
        إذا (وېشل_پایله->اوږدتیا == 0)
          استمر;
        all_new_subtables.push (hb_pair(i, *وېشل_پایله));
        TRY (ګراف_پایله_t<فراغ>::from (all_new_subtables, ویش_FAILURE));
      }
      وإلا
      {
        hb_ویکټور_t<بینښه> new_sub_tables;

        إذا (c.جدول_نښان_2 == HB_OT_نښان_GPOS) {
          بدل (type)
          {
          حالة 2:
            {
              TRY_ټاکل (new_sub_tables, وېشل_subtable<PairPos> (c, subtable_شاخص));
              اكسر;
            }
          حالة 4:
            {
              TRY_ټاکل (new_sub_tables, وېشل_subtable<Mark_بنسټ_Pos> (c, subtable_شاخص));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        } وإلا إذا (c.جدول_نښان_2 == HB_OT_نښان_GSUB) {
          بدل (type)
          {
          حالة 4:
            {
              TRY_ټاکل (new_sub_tables, وېشل_subtable<ګراف::LigatureSubst> (c, subtable_شاخص));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        }

        c.وېشل_subtables.سېټ (subtable_شاخص, new_sub_tables);
        TRY (ګراف_پایله_t<فراغ>::from (c.وېشل_subtables, ویش_FAILURE));
        إذا (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (ګراف_پایله_t<فراغ>::from (all_new_subtables, ویش_FAILURE));
        }
      }
    }

    إذا (all_new_subtables) {
      TRY (جمع_کړه_sub_tables (c, this_شاخص, type, all_new_subtables));
    }

    أرجع Ok();
  }

  کينډۍ<ډول_نوم T>
  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtable (gsubgpos_ګراف_context_t& c,
                                                        بینښه objidx)
  {
    T* sub_جدول_2 = (T*) c.ګراف.object (objidx).سر_2;
    إذا (!sub_جدول_2 || !sub_جدول_2->sanitize (c.ګراف.vertices_[objidx]).is_ok ())
      أرجع Ok(hb_ویکټور_t<بینښه> ());

    أرجع sub_جدول_2->وېشل_subtables (c, objidx);
  }

  ګراف_پایله_t<فراغ> جمع_کړه_sub_tables (gsubgpos_ګراف_context_t& c,
                                       بینښه this_شاخص,
                                       بینښه type,
                                       ثابت hb_ویکټور_t<hb_pair_t<بینښه, hb_ویکټور_t<بینښه>>>& subtable_ids)
  {
    منطقي is_ext = is_extension (c.جدول_نښان_2);
    خپلکاری* v = &c.ګراف.vertices_[this_شاخص];
    fix_existing_subtable_links (c, this_شاخص, subtable_ids);

    بینښه new_subtable_شمېر = 0;
    لكل (ثابت خپلکاری& p : subtable_ids)
      new_subtable_شمېر += p.second.اوږدتیا;

    size_t new_اندازه = v->جدول_اندازه ()
                      + new_subtable_شمېر * OT::Offset16::static_اندازه;
    حرف* بفر = (حرف*) hb_calloc (1, new_اندازه);
    إذا (unlikely (!بفر)) أرجع Err(ویش_FAILURE);
    خپلکاری res = c.جمع_کړه_بفر (بفر);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      أرجع res;
    }
    hb_memcpy (بفر, v->obj ().سر_2, v->جدول_اندازه());

    إذا (use_mark_filtering_سېټ ())
      hb_memcpy (بفر + new_اندازه - 2, v->obj ().لکۍ - 2, 2);

    v->سېټ_بفر (بفر, بفر + new_اندازه);

    Lookup* new_lookup = (Lookup*) بفر;

    بینښه ښويدنه = 0;
    new_lookup->sub_جدول.len = sub_جدول.len + new_subtable_شمېر;
    لكل (ثابت خپلکاری& p : subtable_ids)
    {
      بینښه دننه_واټن_شاخص = p.لومړی + ښويدنه + 1;
      ښويدنه += p.second.اوږدتیا;

      لكل (بینښه subtable_id : p.second)
      {
        إذا (is_ext)
        {
          TRY_ټاکل (بینښه ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.ګراف.vertices_[subtable_id].جمع_کړه_پلار (ext_id, خطأ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.ګراف.vertices_[this_شاخص];
        }

        TRY(v->جمع_کړه_real_link(2, subtable_id,
          (حرف*) &new_lookup->sub_جدول[دننه_واټن_شاخص++] - (حرف*) new_lookup));
        TRY(c.ګراف.vertices_[subtable_id].جمع_کړه_پلار (this_شاخص, خطأ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->ترتیبول_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.سېټ (this_شاخص, new_lookup);
    TRY (ګراف_پایله_t<فراغ>::from (c.lookups, ویش_FAILURE));
    أرجع Ok();
  }

  فراغ fix_existing_subtable_links (gsubgpos_ګراف_context_t& c,
                                    بینښه this_شاخص,
                                    ثابت hb_ویکټور_t<hb_pair_t<بینښه, hb_ویکټور_t<بینښه>>>& subtable_ids)
  {
    خپلکاری& v = c.ګراف.vertices_[this_شاخص];
    بینښه ښويدنه = 0;
    لكل (ثابت خپلکاری& p : subtable_ids)
    {
      بینښه ورننباسه_شاخص = p.لومړی + ښويدنه;
      بینښه pos_دننه_واټن = p.second.اوږدتیا * OT::Offset16::static_اندازه;
      بینښه ورننباسه_دننه_واټن = Lookup::min_اندازه + ورننباسه_شاخص * OT::Offset16::static_اندازه;
      ښويدنه += p.second.اوږدتیا;

      لكل (خپلکاری& l : v.all_links_writer ())
      {
        إذا (l.position > ورننباسه_دننه_واټن) l.position += pos_دننه_واټن;
      }
    }
  }

  ګراف_پایله_t<بینښه> create_extension_subtable (gsubgpos_ګراف_context_t& c,
                                                      بینښه subtable_شاخص,
                                                      بینښه type)
  {
    بینښه extension_اندازه = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_اندازه;

    TRY_ټاکل (بینښه ext_شاخص, c.create_غوټه (extension_اندازه));

    خپلکاری& ext_راس = c.ګراف.vertices_[ext_شاخص];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_راس.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_راس.جمع_کړه_real_link (4, subtable_شاخص, 4));

    أرجع Ok(ext_شاخص);
  }

  ګراف_پایله_t<فراغ> make_subtable_extension (gsubgpos_ګراف_context_t& c,
                                                بینښه lookup_شاخص,
                                                بینښه subtable_شاخص)
  {
    بینښه type = lookupType;
    بینښه ext_شاخص = HB_ګراف_INVALID;
    بینښه* existing_ext_شاخص = تش_اشاره;
    إذا (c.subtable_منزل_extension.has(subtable_شاخص, &existing_ext_شاخص)) {
      ext_شاخص = *existing_ext_شاخص;
    } وإلا {
      TRY_ټاکل (ext_شاخص, create_extension_subtable(c, subtable_شاخص, type));
      c.subtable_منزل_extension.سېټ(subtable_شاخص, ext_شاخص);
      TRY (ګراف_پایله_t<فراغ>::from (c.subtable_منزل_extension, ویش_FAILURE));
    }

    خپلکاری& subtable_راس = c.ګراف.vertices_[subtable_شاخص];
    خپلکاری& lookup_راس = c.ګراف.vertices_[lookup_شاخص];
    بینښه replaced = 0;
    لكل (خپلکاری& l : lookup_راس.real_links_writer ())
    {
      إذا (l.objidx == subtable_شاخص) {
        // Change lookup to point at the extension.
        l.objidx = ext_شاخص;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    خپلکاری& ext_راس = c.ګراف.vertices_[ext_شاخص];
    لكل (بینښه i = 0; i < replaced; i++)
    {
      subtable_راس.remove_پلار (lookup_شاخص);
      TRY(ext_راس.جمع_کړه_پلار (lookup_شاخص, خطأ));
    }
    إذا (!existing_ext_شاخص)
      TRY(subtable_راس.جمع_کړه_پلار (ext_شاخص, خطأ));

    أرجع Ok();
  }

 خاص:
  منطقي is_supported_gsub_type(بینښه type, gsubgpos_ګراف_context_t& c) ثابت {
    أرجع (c.جدول_نښان_2 == HB_OT_نښان_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_جدول::Type::Ligature
    );
  }

  منطقي is_supported_gpos_type(بینښه type, gsubgpos_ګراف_context_t& c) ثابت {
   أرجع (c.جدول_نښان_2 == HB_OT_نښان_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Mark_بنسټ
    );
  }

  بینښه extension_type (hb_نښان_t جدول_نښان_2) ثابت
  {
    بدل (جدول_نښان_2)
    {
    حالة HB_OT_نښان_GPOS: أرجع 9;
    حالة HB_OT_نښان_GSUB: أرجع 7;
    افتراضي: أرجع 0;
    }
  }
};

کينډۍ <ډول_نوم T>
بنية LookupList : عام OT::LookupList<T>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < OT::LookupList<T>::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < OT::LookupList<T>::توکی_اندازه * هذا->len)) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية GSTAR : عام OT::GSUBGPOS
{
  ساكن ګراف_پایله_t<ثابت GSTAR*> ګراف_منزل_gstar (ګراف_t& ګراف)
  {
    ثابت خپلکاری& r = ګراف.ریښه ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().سر_2;
    إذا (unlikely (!gstar)) أرجع Err(INVALID_دلیل);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    أرجع gstar;
  }

  ثابت فراغ* get_lookup_list_ډګر_دننه_واټن () ثابت
  {
    أرجع OT::GSUBGPOS::get_lookup_list_ډګر_دننه_واټن ();
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t len = راس.جدول_اندازه ();
    إذا (unlikely (len < OT::GSUBGPOS::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (نسخه.major != 1)) أرجع Err(SANITIZE_FAILURE);
    إذا (unlikely (len < get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> پیدا_کړه_lookups (ګراف_t& ګراف,
                                     hb_hashmap_t<بینښه, Lookup*>& lookups /*بهر*/) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && lookupList2)
      أرجع پیدا_کړه_lookups<MediumTypes> (ګراف, lookups);
#د_که_پای
    TRY(پیدا_کړه_lookups<SmallTypes> (ګراف, lookups));
    أرجع Ok();
  }

  ګراف_پایله_t<بینښه> get_lookup_list_شاخص (ګراف_t& ګراف) ثابت
  {
    ثابت فراغ* دننه_واټن = get_lookup_list_ډګر_دننه_واټن ();
    إذا (unlikely (!دننه_واټن)) أرجع Err(INVALID_دلیل);
    أرجع ګراف.شاخص_for_دننه_واټن (ګراف.ریښه_idx (), دننه_واټن);
  }

  کينډۍ<ډول_نوم Types>
  ګراف_پایله_t<فراغ> پیدا_کړه_lookups (ګراف_t& ګراف,
                                     hb_hashmap_t<بینښه, Lookup*>& lookups /*بهر*/) ثابت
  {
    TRY_ټاکل (بینښه lookup_list_idx, get_lookup_list_شاخص (ګراف));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) ګراف.object (lookup_list_idx).سر_2;
    إذا (unlikely (!lookupList)) أرجع Err(INVALID_دلیل);
    TRY(lookupList->sanitize (ګراف.vertices_[lookup_list_idx]));

    لكل (بینښه i = 0; i < lookupList->len; i++)
    {
      TRY_ټاکل (بینښه lookup_idx, ګراف.شاخص_for_دننه_واټن (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) ګراف.object (lookup_idx).سر_2;
      إذا (!lookup || !lookup->sanitize (ګراف.vertices_[lookup_idx]).is_ok ()) استمر;
      lookups.سېټ (lookup_idx, lookup);
    }
    أرجع Ok();
  }
};




}

#د_که_پای  /* GRAPH_GSUBGPOS_GRAPH_HH */
