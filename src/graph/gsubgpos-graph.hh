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

#اگر_تعریف_نشده گراف_GSUBGPOS_گراف_HH
#تعریف_کن گراف_GSUBGPOS_گراف_HH

نطاق گراف {

بنية Lookup;

قالب<نام_نوع T>
بنية ExtensionFormat1 : عام OT::ExtensionFormat1<T>
{
  فراغ reset(بیعلامت type)
  {
    هذا->format = 1;
    هذا->extensionLookupType = type;
    هذا->extension_جابه_جایی = 0;
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < OT::ExtensionFormat1<T>::static_اندازه))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  بیعلامت get_lookup_type () ثابت
  {
    أرجع هذا->extensionLookupType;
  }

  گراف_نتیجه_t<بیعلامت> get_subtable_شاخص (گراف_t& گراف, بیعلامت this_شاخص) ثابت
  {
    أرجع گراف.شاخص_for_جابه_جایی (this_شاخص, &هذا->extension_جابه_جایی);
  }
};

بنية Lookup : عام OT::Lookup
{
  بیعلامت عدد_از_subtables () ثابت
  {
    أرجع sub_جدول.len;
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < OT::Lookup::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (رأس_len < هذا->get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  منطقي is_extension (hb_برچسب_t جدول_برچسب_2) ثابت
  {
    أرجع lookupType == extension_type (جدول_برچسب_2);
  }

  منطقي use_mark_filtering_مجموعه () ثابت
  {
    بیعلامت flag = lookupFlag;
    أرجع flag & 0x0010u;
  }

  گراف_نتیجه_t<فراغ> make_extension (gsubgpos_گراف_context_t& c,
                                       بیعلامت this_شاخص)
  {
    بیعلامت type = lookupType;
    بیعلامت ext_type = extension_type (c.جدول_برچسب_2);
    إذا (!ext_type || is_extension (c.جدول_برچسب_2))
    {
      // NOOP
      أرجع Ok();
    }

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_شاخص);

    hb_مجموعه_t دیده;
    hb_وکتور_t<بیعلامت> subtables;
    لكل (بیعلامت i = 0; i < sub_جدول.len; i++)
    {
      TRY_تخصیص_بده (بیعلامت subtable_شاخص, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &sub_جدول[i]));
      إذا (دیده.has (subtable_شاخص))
        استمر;
      دیده.جمع_کن (subtable_شاخص);
      subtables.push (subtable_شاخص);
    }
    TRY (گراف_نتیجه_t<فراغ>::from (دیده, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (subtables, تخصیص_FAILURE));

    لكل (بیعلامت subtable_شاخص : subtables)
      TRY (make_subtable_extension (c, this_شاخص, subtable_شاخص));

    lookupType = ext_type;
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> تقسیم_subtables_if_needed (gsubgpos_گراف_context_t& c,
                                                  بیعلامت this_شاخص)
  {
    بیعلامت type = lookupType;
    منطقي is_ext = is_extension (c.جدول_برچسب_2);

    إذا (c.جدول_برچسب_2 != HB_OT_برچسب_GPOS && c.جدول_برچسب_2 != HB_OT_برچسب_GSUB)
      أرجع Ok();

    إذا (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      أرجع Ok();

    hb_وکتور_t<hb_pair_t<بیعلامت, hb_وکتور_t<بیعلامت>>> all_new_subtables;
    لكل (بیعلامت i = 0; i < sub_جدول.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      خودکار idx_res = c.گراف.شاخص_for_جابه_جایی (this_شاخص, &sub_جدول[i]);
      إذا (!idx_res.is_ok ()) استمر;
      بیعلامت subtable_شاخص = *idx_res;
      إذا (is_ext) {
        بیعلامت ext_subtable_شاخص = subtable_شاخص;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.گراف.object (ext_subtable_شاخص).سر_2;
        إذا (!extension || !extension->sanitize (c.گراف.vertices_[ext_subtable_شاخص]).is_ok ())
          استمر;

        TRY_تخصیص_بده (subtable_شاخص, extension->get_subtable_شاخص (c.گراف, ext_subtable_شاخص));
        type = extension->get_lookup_type ();
        إذا (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          استمر;
      }

      hb_وکتور_t<بیعلامت>* تقسیم_نتیجه;
      إذا (c.تقسیم_subtables.has (subtable_شاخص, &تقسیم_نتیجه))
      {
        إذا (تقسیم_نتیجه->درازی == 0)
          استمر;
        all_new_subtables.push (hb_pair(i, *تقسیم_نتیجه));
        TRY (گراف_نتیجه_t<فراغ>::from (all_new_subtables, تخصیص_FAILURE));
      }
      وإلا
      {
        hb_وکتور_t<بیعلامت> new_sub_tables;

        إذا (c.جدول_برچسب_2 == HB_OT_برچسب_GPOS) {
          بدل (type)
          {
          حالة 2:
            {
              TRY_تخصیص_بده (new_sub_tables, تقسیم_subtable<PairPos> (c, subtable_شاخص));
              اكسر;
            }
          حالة 4:
            {
              TRY_تخصیص_بده (new_sub_tables, تقسیم_subtable<Mark_پایه_Pos> (c, subtable_شاخص));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        } وإلا إذا (c.جدول_برچسب_2 == HB_OT_برچسب_GSUB) {
          بدل (type)
          {
          حالة 4:
            {
              TRY_تخصیص_بده (new_sub_tables, تقسیم_subtable<گراف::LigatureSubst> (c, subtable_شاخص));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        }

        c.تقسیم_subtables.مجموعه (subtable_شاخص, new_sub_tables);
        TRY (گراف_نتیجه_t<فراغ>::from (c.تقسیم_subtables, تخصیص_FAILURE));
        إذا (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (گراف_نتیجه_t<فراغ>::from (all_new_subtables, تخصیص_FAILURE));
        }
      }
    }

    إذا (all_new_subtables) {
      TRY (جمع_کن_sub_tables (c, this_شاخص, type, all_new_subtables));
    }

    أرجع Ok();
  }

  قالب<نام_نوع T>
  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtable (gsubgpos_گراف_context_t& c,
                                                        بیعلامت objidx)
  {
    T* sub_جدول_2 = (T*) c.گراف.object (objidx).سر_2;
    إذا (!sub_جدول_2 || !sub_جدول_2->sanitize (c.گراف.vertices_[objidx]).is_ok ())
      أرجع Ok(hb_وکتور_t<بیعلامت> ());

    أرجع sub_جدول_2->تقسیم_subtables (c, objidx);
  }

  گراف_نتیجه_t<فراغ> جمع_کن_sub_tables (gsubgpos_گراف_context_t& c,
                                       بیعلامت this_شاخص,
                                       بیعلامت type,
                                       ثابت hb_وکتور_t<hb_pair_t<بیعلامت, hb_وکتور_t<بیعلامت>>>& subtable_ids)
  {
    منطقي is_ext = is_extension (c.جدول_برچسب_2);
    خودکار* v = &c.گراف.vertices_[this_شاخص];
    fix_existing_subtable_links (c, this_شاخص, subtable_ids);

    بیعلامت new_subtable_شمار = 0;
    لكل (ثابت خودکار& p : subtable_ids)
      new_subtable_شمار += p.second.درازی;

    size_t new_اندازه = v->جدول_اندازه ()
                      + new_subtable_شمار * OT::Offset16::static_اندازه;
    حرف* میانگیر = (حرف*) hb_calloc (1, new_اندازه);
    إذا (unlikely (!میانگیر)) أرجع Err(تخصیص_FAILURE);
    خودکار res = c.جمع_کن_میانگیر (میانگیر);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (میانگیر);
      أرجع res;
    }
    hb_memcpy (میانگیر, v->obj ().سر_2, v->جدول_اندازه());

    إذا (use_mark_filtering_مجموعه ())
      hb_memcpy (میانگیر + new_اندازه - 2, v->obj ().دم - 2, 2);

    v->مجموعه_میانگیر (میانگیر, میانگیر + new_اندازه);

    Lookup* new_lookup = (Lookup*) میانگیر;

    بیعلامت لغزش = 0;
    new_lookup->sub_جدول.len = sub_جدول.len + new_subtable_شمار;
    لكل (ثابت خودکار& p : subtable_ids)
    {
      بیعلامت جابه_جایی_شاخص = p.اولی + لغزش + 1;
      لغزش += p.second.درازی;

      لكل (بیعلامت subtable_id : p.second)
      {
        إذا (is_ext)
        {
          TRY_تخصیص_بده (بیعلامت ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.گراف.vertices_[subtable_id].جمع_کن_والد (ext_id, خطأ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.گراف.vertices_[this_شاخص];
        }

        TRY(v->جمع_کن_real_link(2, subtable_id,
          (حرف*) &new_lookup->sub_جدول[جابه_جایی_شاخص++] - (حرف*) new_lookup));
        TRY(c.گراف.vertices_[subtable_id].جمع_کن_والد (this_شاخص, خطأ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->مرتب_سازی_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.مجموعه (this_شاخص, new_lookup);
    TRY (گراف_نتیجه_t<فراغ>::from (c.lookups, تخصیص_FAILURE));
    أرجع Ok();
  }

  فراغ fix_existing_subtable_links (gsubgpos_گراف_context_t& c,
                                    بیعلامت this_شاخص,
                                    ثابت hb_وکتور_t<hb_pair_t<بیعلامت, hb_وکتور_t<بیعلامت>>>& subtable_ids)
  {
    خودکار& v = c.گراف.vertices_[this_شاخص];
    بیعلامت لغزش = 0;
    لكل (ثابت خودکار& p : subtable_ids)
    {
      بیعلامت درج_شاخص = p.اولی + لغزش;
      بیعلامت pos_جابه_جایی = p.second.درازی * OT::Offset16::static_اندازه;
      بیعلامت درج_جابه_جایی = Lookup::min_اندازه + درج_شاخص * OT::Offset16::static_اندازه;
      لغزش += p.second.درازی;

      لكل (خودکار& l : v.all_links_writer ())
      {
        إذا (l.position > درج_جابه_جایی) l.position += pos_جابه_جایی;
      }
    }
  }

  گراف_نتیجه_t<بیعلامت> create_extension_subtable (gsubgpos_گراف_context_t& c,
                                                      بیعلامت subtable_شاخص,
                                                      بیعلامت type)
  {
    بیعلامت extension_اندازه = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_اندازه;

    TRY_تخصیص_بده (بیعلامت ext_شاخص, c.create_گره (extension_اندازه));

    خودکار& ext_رأس = c.گراف.vertices_[ext_شاخص];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_رأس.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_رأس.جمع_کن_real_link (4, subtable_شاخص, 4));

    أرجع Ok(ext_شاخص);
  }

  گراف_نتیجه_t<فراغ> make_subtable_extension (gsubgpos_گراف_context_t& c,
                                                بیعلامت lookup_شاخص,
                                                بیعلامت subtable_شاخص)
  {
    بیعلامت type = lookupType;
    بیعلامت ext_شاخص = HB_گراف_INVALID;
    بیعلامت* existing_ext_شاخص = اشاره_تهی;
    إذا (c.subtable_مقصد_extension.has(subtable_شاخص, &existing_ext_شاخص)) {
      ext_شاخص = *existing_ext_شاخص;
    } وإلا {
      TRY_تخصیص_بده (ext_شاخص, create_extension_subtable(c, subtable_شاخص, type));
      c.subtable_مقصد_extension.مجموعه(subtable_شاخص, ext_شاخص);
      TRY (گراف_نتیجه_t<فراغ>::from (c.subtable_مقصد_extension, تخصیص_FAILURE));
    }

    خودکار& subtable_رأس = c.گراف.vertices_[subtable_شاخص];
    خودکار& lookup_رأس = c.گراف.vertices_[lookup_شاخص];
    بیعلامت replaced = 0;
    لكل (خودکار& l : lookup_رأس.real_links_writer ())
    {
      إذا (l.objidx == subtable_شاخص) {
        // Change lookup to point at the extension.
        l.objidx = ext_شاخص;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    خودکار& ext_رأس = c.گراف.vertices_[ext_شاخص];
    لكل (بیعلامت i = 0; i < replaced; i++)
    {
      subtable_رأس.remove_والد (lookup_شاخص);
      TRY(ext_رأس.جمع_کن_والد (lookup_شاخص, خطأ));
    }
    إذا (!existing_ext_شاخص)
      TRY(subtable_رأس.جمع_کن_والد (ext_شاخص, خطأ));

    أرجع Ok();
  }

 خاص:
  منطقي is_supported_gsub_type(بیعلامت type, gsubgpos_گراف_context_t& c) ثابت {
    أرجع (c.جدول_برچسب_2 == HB_OT_برچسب_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_جدول::Type::Ligature
    );
  }

  منطقي is_supported_gpos_type(بیعلامت type, gsubgpos_گراف_context_t& c) ثابت {
   أرجع (c.جدول_برچسب_2 == HB_OT_برچسب_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Mark_پایه
    );
  }

  بیعلامت extension_type (hb_برچسب_t جدول_برچسب_2) ثابت
  {
    بدل (جدول_برچسب_2)
    {
    حالة HB_OT_برچسب_GPOS: أرجع 9;
    حالة HB_OT_برچسب_GSUB: أرجع 7;
    افتراضي: أرجع 0;
    }
  }
};

قالب <نام_نوع T>
بنية LookupList : عام OT::LookupList<T>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < OT::LookupList<T>::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (رأس_len < OT::LookupList<T>::قلم_اندازه * هذا->len)) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية GSTAR : عام OT::GSUBGPOS
{
  ساكن گراف_نتیجه_t<ثابت GSTAR*> گراف_مقصد_gstar (گراف_t& گراف)
  {
    ثابت خودکار& r = گراف.ریشه ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().سر_2;
    إذا (unlikely (!gstar)) أرجع Err(INVALID_آرگومان);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    أرجع gstar;
  }

  ثابت فراغ* get_lookup_list_فیلد_جابه_جایی () ثابت
  {
    أرجع OT::GSUBGPOS::get_lookup_list_فیلد_جابه_جایی ();
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t len = رأس.جدول_اندازه ();
    إذا (unlikely (len < OT::GSUBGPOS::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (نسخه.major != 1)) أرجع Err(SANITIZE_FAILURE);
    إذا (unlikely (len < get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> پیدا_کن_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بیعلامت, Lookup*>& lookups /*بیرون*/) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && lookupList2)
      أرجع پیدا_کن_lookups<MediumTypes> (گراف, lookups);
#پایان_اگر
    TRY(پیدا_کن_lookups<SmallTypes> (گراف, lookups));
    أرجع Ok();
  }

  گراف_نتیجه_t<بیعلامت> get_lookup_list_شاخص (گراف_t& گراف) ثابت
  {
    ثابت فراغ* جابه_جایی = get_lookup_list_فیلد_جابه_جایی ();
    إذا (unlikely (!جابه_جایی)) أرجع Err(INVALID_آرگومان);
    أرجع گراف.شاخص_for_جابه_جایی (گراف.ریشه_idx (), جابه_جایی);
  }

  قالب<نام_نوع Types>
  گراف_نتیجه_t<فراغ> پیدا_کن_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بیعلامت, Lookup*>& lookups /*بیرون*/) ثابت
  {
    TRY_تخصیص_بده (بیعلامت lookup_list_idx, get_lookup_list_شاخص (گراف));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) گراف.object (lookup_list_idx).سر_2;
    إذا (unlikely (!lookupList)) أرجع Err(INVALID_آرگومان);
    TRY(lookupList->sanitize (گراف.vertices_[lookup_list_idx]));

    لكل (بیعلامت i = 0; i < lookupList->len; i++)
    {
      TRY_تخصیص_بده (بیعلامت lookup_idx, گراف.شاخص_for_جابه_جایی (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) گراف.object (lookup_idx).سر_2;
      إذا (!lookup || !lookup->sanitize (گراف.vertices_[lookup_idx]).is_ok ()) استمر;
      lookups.مجموعه (lookup_idx, lookup);
    }
    أرجع Ok();
  }
};




}

#پایان_اگر  /* GRAPH_GSUBGPOS_GRAPH_HH */
