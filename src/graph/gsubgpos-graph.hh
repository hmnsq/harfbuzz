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

#جے_بنا_تعریف گراف_GSUBGPOS_گراف_HH
#تعریف_کرو گراف_GSUBGPOS_گراف_HH

نطاق گراف {

بنية Lookup;

سانچا<قسم_ناں T>
بنية ExtensionFormat1 : عام OT::ExtensionFormat1<T>
{
  فراغ reset(بنانشانی type)
  {
    هذا->format = 1;
    هذا->extensionLookupType = type;
    هذا->extension_وتھ = 0;
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < OT::ExtensionFormat1<T>::static_ناپ))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  بنانشانی get_lookup_type () ثابت
  {
    أرجع هذا->extensionLookupType;
  }

  گراف_نتیجہ_t<بنانشانی> get_subtable_اشاریہ (گراف_t& گراف, بنانشانی this_اشاریہ) ثابت
  {
    أرجع گراف.اشاریہ_for_وتھ (this_اشاریہ, &هذا->extension_وتھ);
  }
};

بنية Lookup : عام OT::Lookup
{
  بنانشانی نمبر_دا_subtables () ثابت
  {
    أرجع sub_جدول.len;
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < OT::Lookup::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < هذا->get_ناپ ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  منطقي is_extension (hb_ٹیگ_t جدول_ٹیگ_2) ثابت
  {
    أرجع lookupType == extension_type (جدول_ٹیگ_2);
  }

  منطقي use_mark_filtering_سیٹ () ثابت
  {
    بنانشانی flag = lookupFlag;
    أرجع flag & 0x0010u;
  }

  گراف_نتیجہ_t<فراغ> make_extension (gsubgpos_گراف_context_t& c,
                                       بنانشانی this_اشاریہ)
  {
    بنانشانی type = lookupType;
    بنانشانی ext_type = extension_type (c.جدول_ٹیگ_2);
    إذا (!ext_type || is_extension (c.جدول_ٹیگ_2))
    {
      // NOOP
      أرجع Ok();
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_اشاریہ);

    hb_سیٹ_t تکیا;
    hb_سمتیہ_t<بنانشانی> subtables;
    لكل (بنانشانی i = 0; i < sub_جدول.len; i++)
    {
      TRY_سونپو (بنانشانی subtable_اشاریہ, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &sub_جدول[i]));
      إذا (تکیا.has (subtable_اشاریہ))
        استمر;
      تکیا.جوڑو (subtable_اشاریہ);
      subtables.push (subtable_اشاریہ);
    }
    TRY (گراف_نتیجہ_t<فراغ>::from (تکیا, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (subtables, ونڈ_FAILURE));

    لكل (بنانشانی subtable_اشاریہ : subtables)
      TRY (make_subtable_extension (c, this_اشاریہ, subtable_اشاریہ));

    lookupType = ext_type;
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> ونڈو_subtables_if_needed (gsubgpos_گراف_context_t& c,
                                                  بنانشانی this_اشاریہ)
  {
    بنانشانی type = lookupType;
    منطقي is_ext = is_extension (c.جدول_ٹیگ_2);

    إذا (c.جدول_ٹیگ_2 != HB_OT_ٹیگ_GPOS && c.جدول_ٹیگ_2 != HB_OT_ٹیگ_GSUB)
      أرجع Ok();

    إذا (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      أرجع Ok();

    hb_سمتیہ_t<hb_pair_t<بنانشانی, hb_سمتیہ_t<بنانشانی>>> all_new_subtables;
    لكل (بنانشانی i = 0; i < sub_جدول.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      خودکار idx_res = c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &sub_جدول[i]);
      إذا (!idx_res.is_ok ()) استمر;
      بنانشانی subtable_اشاریہ = *idx_res;
      إذا (is_ext) {
        بنانشانی ext_subtable_اشاریہ = subtable_اشاریہ;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.گراف.object (ext_subtable_اشاریہ).سر_2;
        إذا (!extension || !extension->sanitize (c.گراف.vertices_[ext_subtable_اشاریہ]).is_ok ())
          استمر;

        TRY_سونپو (subtable_اشاریہ, extension->get_subtable_اشاریہ (c.گراف, ext_subtable_اشاریہ));
        type = extension->get_lookup_type ();
        إذا (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          استمر;
      }

      hb_سمتیہ_t<بنانشانی>* ونڈو_نتیجہ;
      إذا (c.ونڈو_subtables.has (subtable_اشاریہ, &ونڈو_نتیجہ))
      {
        إذا (ونڈو_نتیجہ->لمائی == 0)
          استمر;
        all_new_subtables.push (hb_pair(i, *ونڈو_نتیجہ));
        TRY (گراف_نتیجہ_t<فراغ>::from (all_new_subtables, ونڈ_FAILURE));
      }
      وإلا
      {
        hb_سمتیہ_t<بنانشانی> new_sub_tables;

        إذا (c.جدول_ٹیگ_2 == HB_OT_ٹیگ_GPOS) {
          بدل (type)
          {
          حالة 2:
            {
              TRY_سونپو (new_sub_tables, ونڈو_subtable<PairPos> (c, subtable_اشاریہ));
              اكسر;
            }
          حالة 4:
            {
              TRY_سونپو (new_sub_tables, ونڈو_subtable<Mark_ادھار_Pos> (c, subtable_اشاریہ));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        } وإلا إذا (c.جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB) {
          بدل (type)
          {
          حالة 4:
            {
              TRY_سونپو (new_sub_tables, ونڈو_subtable<گراف::LigatureSubst> (c, subtable_اشاریہ));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        }

        c.ونڈو_subtables.سیٹ (subtable_اشاریہ, new_sub_tables);
        TRY (گراف_نتیجہ_t<فراغ>::from (c.ونڈو_subtables, ونڈ_FAILURE));
        إذا (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (گراف_نتیجہ_t<فراغ>::from (all_new_subtables, ونڈ_FAILURE));
        }
      }
    }

    إذا (all_new_subtables) {
      TRY (جوڑو_sub_tables (c, this_اشاریہ, type, all_new_subtables));
    }

    أرجع Ok();
  }

  سانچا<قسم_ناں T>
  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtable (gsubgpos_گراف_context_t& c,
                                                        بنانشانی objidx)
  {
    T* sub_جدول_2 = (T*) c.گراف.object (objidx).سر_2;
    إذا (!sub_جدول_2 || !sub_جدول_2->sanitize (c.گراف.vertices_[objidx]).is_ok ())
      أرجع Ok(hb_سمتیہ_t<بنانشانی> ());

    أرجع sub_جدول_2->ونڈو_subtables (c, objidx);
  }

  گراف_نتیجہ_t<فراغ> جوڑو_sub_tables (gsubgpos_گراف_context_t& c,
                                       بنانشانی this_اشاریہ,
                                       بنانشانی type,
                                       ثابت hb_سمتیہ_t<hb_pair_t<بنانشانی, hb_سمتیہ_t<بنانشانی>>>& subtable_ids)
  {
    منطقي is_ext = is_extension (c.جدول_ٹیگ_2);
    خودکار* v = &c.گراف.vertices_[this_اشاریہ];
    fix_existing_subtable_links (c, this_اشاریہ, subtable_ids);

    بنانشانی new_subtable_گنتی = 0;
    لكل (ثابت خودکار& p : subtable_ids)
      new_subtable_گنتی += p.second.لمائی;

    size_t new_ناپ = v->جدول_ناپ ()
                      + new_subtable_گنتی * OT::Offset16::static_ناپ;
    حرف* بفر = (حرف*) hb_calloc (1, new_ناپ);
    إذا (unlikely (!بفر)) أرجع Err(ونڈ_FAILURE);
    خودکار res = c.جوڑو_بفر (بفر);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      أرجع res;
    }
    hb_memcpy (بفر, v->obj ().سر_2, v->جدول_ناپ());

    إذا (use_mark_filtering_سیٹ ())
      hb_memcpy (بفر + new_ناپ - 2, v->obj ().پوچھ - 2, 2);

    v->سیٹ_بفر (بفر, بفر + new_ناپ);

    Lookup* new_lookup = (Lookup*) بفر;

    بنانشانی کھسکاؤ = 0;
    new_lookup->sub_جدول.len = sub_جدول.len + new_subtable_گنتی;
    لكل (ثابت خودکار& p : subtable_ids)
    {
      بنانشانی وتھ_اشاریہ = p.پہلا + کھسکاؤ + 1;
      کھسکاؤ += p.second.لمائی;

      لكل (بنانشانی subtable_id : p.second)
      {
        إذا (is_ext)
        {
          TRY_سونپو (بنانشانی ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.گراف.vertices_[subtable_id].جوڑو_ماپا (ext_id, خطأ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.گراف.vertices_[this_اشاریہ];
        }

        TRY(v->جوڑو_real_link(2, subtable_id,
          (حرف*) &new_lookup->sub_جدول[وتھ_اشاریہ++] - (حرف*) new_lookup));
        TRY(c.گراف.vertices_[subtable_id].جوڑو_ماپا (this_اشاریہ, خطأ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->ترتیب_دیو_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.سیٹ (this_اشاریہ, new_lookup);
    TRY (گراف_نتیجہ_t<فراغ>::from (c.lookups, ونڈ_FAILURE));
    أرجع Ok();
  }

  فراغ fix_existing_subtable_links (gsubgpos_گراف_context_t& c,
                                    بنانشانی this_اشاریہ,
                                    ثابت hb_سمتیہ_t<hb_pair_t<بنانشانی, hb_سمتیہ_t<بنانشانی>>>& subtable_ids)
  {
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    بنانشانی کھسکاؤ = 0;
    لكل (ثابت خودکار& p : subtable_ids)
    {
      بنانشانی داخل_کرو_اشاریہ = p.پہلا + کھسکاؤ;
      بنانشانی pos_وتھ = p.second.لمائی * OT::Offset16::static_ناپ;
      بنانشانی داخل_کرو_وتھ = Lookup::min_ناپ + داخل_کرو_اشاریہ * OT::Offset16::static_ناپ;
      کھسکاؤ += p.second.لمائی;

      لكل (خودکار& l : v.all_links_writer ())
      {
        إذا (l.position > داخل_کرو_وتھ) l.position += pos_وتھ;
      }
    }
  }

  گراف_نتیجہ_t<بنانشانی> create_extension_subtable (gsubgpos_گراف_context_t& c,
                                                      بنانشانی subtable_اشاریہ,
                                                      بنانشانی type)
  {
    بنانشانی extension_ناپ = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_ناپ;

    TRY_سونپو (بنانشانی ext_اشاریہ, c.create_گرہ (extension_ناپ));

    خودکار& ext_راس = c.گراف.vertices_[ext_اشاریہ];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_راس.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_راس.جوڑو_real_link (4, subtable_اشاریہ, 4));

    أرجع Ok(ext_اشاریہ);
  }

  گراف_نتیجہ_t<فراغ> make_subtable_extension (gsubgpos_گراف_context_t& c,
                                                بنانشانی lookup_اشاریہ,
                                                بنانشانی subtable_اشاریہ)
  {
    بنانشانی type = lookupType;
    بنانشانی ext_اشاریہ = HB_گراف_INVALID;
    بنانشانی* existing_ext_اشاریہ = خالی_سوچک;
    إذا (c.subtable_منزل_extension.has(subtable_اشاریہ, &existing_ext_اشاریہ)) {
      ext_اشاریہ = *existing_ext_اشاریہ;
    } وإلا {
      TRY_سونپو (ext_اشاریہ, create_extension_subtable(c, subtable_اشاریہ, type));
      c.subtable_منزل_extension.سیٹ(subtable_اشاریہ, ext_اشاریہ);
      TRY (گراف_نتیجہ_t<فراغ>::from (c.subtable_منزل_extension, ونڈ_FAILURE));
    }

    خودکار& subtable_راس = c.گراف.vertices_[subtable_اشاریہ];
    خودکار& lookup_راس = c.گراف.vertices_[lookup_اشاریہ];
    بنانشانی replaced = 0;
    لكل (خودکار& l : lookup_راس.real_links_writer ())
    {
      إذا (l.objidx == subtable_اشاریہ) {
        // Change lookup to point at the extension.
        l.objidx = ext_اشاریہ;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    خودکار& ext_راس = c.گراف.vertices_[ext_اشاریہ];
    لكل (بنانشانی i = 0; i < replaced; i++)
    {
      subtable_راس.remove_ماپا (lookup_اشاریہ);
      TRY(ext_راس.جوڑو_ماپا (lookup_اشاریہ, خطأ));
    }
    إذا (!existing_ext_اشاریہ)
      TRY(subtable_راس.جوڑو_ماپا (ext_اشاریہ, خطأ));

    أرجع Ok();
  }

 خاص:
  منطقي is_supported_gsub_type(بنانشانی type, gsubgpos_گراف_context_t& c) ثابت {
    أرجع (c.جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_جدول::Type::Ligature
    );
  }

  منطقي is_supported_gpos_type(بنانشانی type, gsubgpos_گراف_context_t& c) ثابت {
   أرجع (c.جدول_ٹیگ_2 == HB_OT_ٹیگ_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Mark_ادھار
    );
  }

  بنانشانی extension_type (hb_ٹیگ_t جدول_ٹیگ_2) ثابت
  {
    بدل (جدول_ٹیگ_2)
    {
    حالة HB_OT_ٹیگ_GPOS: أرجع 9;
    حالة HB_OT_ٹیگ_GSUB: أرجع 7;
    افتراضي: أرجع 0;
    }
  }
};

سانچا <قسم_ناں T>
بنية LookupList : عام OT::LookupList<T>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < OT::LookupList<T>::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < OT::LookupList<T>::شے_ناپ * هذا->len)) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية GSTAR : عام OT::GSUBGPOS
{
  ساكن گراف_نتیجہ_t<ثابت GSTAR*> گراف_منزل_gstar (گراف_t& گراف)
  {
    ثابت خودکار& r = گراف.جڑ ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().سر_2;
    إذا (unlikely (!gstar)) أرجع Err(INVALID_دلیل);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    أرجع gstar;
  }

  ثابت فراغ* get_lookup_list_میدان_وتھ () ثابت
  {
    أرجع OT::GSUBGPOS::get_lookup_list_میدان_وتھ ();
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t len = راس.جدول_ناپ ();
    إذا (unlikely (len < OT::GSUBGPOS::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (نسخہ.major != 1)) أرجع Err(SANITIZE_FAILURE);
    إذا (unlikely (len < get_ناپ ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> لبھو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بنانشانی, Lookup*>& lookups /*باہرلا*/) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && lookupList2)
      أرجع لبھو_lookups<MediumTypes> (گراف, lookups);
#جے_انت
    TRY(لبھو_lookups<SmallTypes> (گراف, lookups));
    أرجع Ok();
  }

  گراف_نتیجہ_t<بنانشانی> get_lookup_list_اشاریہ (گراف_t& گراف) ثابت
  {
    ثابت فراغ* وتھ = get_lookup_list_میدان_وتھ ();
    إذا (unlikely (!وتھ)) أرجع Err(INVALID_دلیل);
    أرجع گراف.اشاریہ_for_وتھ (گراف.جڑ_idx (), وتھ);
  }

  سانچا<قسم_ناں Types>
  گراف_نتیجہ_t<فراغ> لبھو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<بنانشانی, Lookup*>& lookups /*باہرلا*/) ثابت
  {
    TRY_سونپو (بنانشانی lookup_list_idx, get_lookup_list_اشاریہ (گراف));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) گراف.object (lookup_list_idx).سر_2;
    إذا (unlikely (!lookupList)) أرجع Err(INVALID_دلیل);
    TRY(lookupList->sanitize (گراف.vertices_[lookup_list_idx]));

    لكل (بنانشانی i = 0; i < lookupList->len; i++)
    {
      TRY_سونپو (بنانشانی lookup_idx, گراف.اشاریہ_for_وتھ (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) گراف.object (lookup_idx).سر_2;
      إذا (!lookup || !lookup->sanitize (گراف.vertices_[lookup_idx]).is_ok ()) استمر;
      lookups.سیٹ (lookup_idx, lookup);
    }
    أرجع Ok();
  }
};




}

#جے_انت  /* GRAPH_GSUBGPOS_GRAPH_HH */
