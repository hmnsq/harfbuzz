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

#شامل "graph.hh"
#شامل "../hb-ot-layout-gsubgpos.hh"
#شامل "../OT/Layout/GSUB/ExtensionSubst.hh"
#شامل "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#شامل "gsubgpos-context.hh"
#شامل "pairpos-graph.hh"
#شامل "markbasepos-graph.hh"
#شامل "ligature-graph.hh"

#اگر_غیر_تعریف_شدہ گراف_GSUBGPOS_گراف_HH
#تعریف_کرو گراف_GSUBGPOS_گراف_HH

نامخلا گراف {

ساخت Lookup;

سانچہ<قسم_نام T>
ساخت ExtensionFormat1 : عوامی OT::ExtensionFormat1<T>
{
  خالی reset(غیرعلامتی type)
  {
    یہ->format = 1;
    یہ->extensionLookupType = type;
    یہ->extension_انحراف = 0;
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < OT::ExtensionFormat1<T>::static_سائز))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  غیرعلامتی get_lookup_type () ثابت
  {
    واپس یہ->extensionLookupType;
  }

  گراف_نتیجہ_t<غیرعلامتی> get_subtable_اشاریہ (گراف_t& گراف, غیرعلامتی this_اشاریہ) ثابت
  {
    واپس گراف.اشاریہ_for_انحراف (this_اشاریہ, &یہ->extension_انحراف);
  }
};

ساخت Lookup : عوامی OT::Lookup
{
  غیرعلامتی نمبر_کا_subtables () ثابت
  {
    واپس sub_جدول.len;
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < OT::Lookup::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (راس_len < یہ->get_سائز ())) واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  منطقی is_extension (hb_لیبل_t جدول_لیبل_2) ثابت
  {
    واپس lookupType == extension_type (جدول_لیبل_2);
  }

  منطقی use_mark_filtering_سیٹ () ثابت
  {
    غیرعلامتی flag = lookupFlag;
    واپس flag & 0x0010u;
  }

  گراف_نتیجہ_t<خالی> make_extension (gsubgpos_گراف_context_t& c,
                                       غیرعلامتی this_اشاریہ)
  {
    غیرعلامتی type = lookupType;
    غیرعلامتی ext_type = extension_type (c.جدول_لیبل_2);
    اگر (!ext_type || is_extension (c.جدول_لیبل_2))
    {
      // NOOP
      واپس Ok();
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_اشاریہ);

    hb_سیٹ_t نظر_آیا;
    hb_سمتیہ_t<غیرعلامتی> subtables;
    برائے (غیرعلامتی i = 0; i < sub_جدول.len; i++)
    {
      TRY_تفویض (غیرعلامتی subtable_اشاریہ, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &sub_جدول[i]));
      اگر (نظر_آیا.has (subtable_اشاریہ))
        جاری;
      نظر_آیا.جوڑو (subtable_اشاریہ);
      subtables.push (subtable_اشاریہ);
    }
    TRY (گراف_نتیجہ_t<خالی>::from (نظر_آیا, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (subtables, تخصیص_FAILURE));

    برائے (غیرعلامتی subtable_اشاریہ : subtables)
      TRY (make_subtable_extension (c, this_اشاریہ, subtable_اشاریہ));

    lookupType = ext_type;
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> بانٹو_subtables_if_needed (gsubgpos_گراف_context_t& c,
                                                  غیرعلامتی this_اشاریہ)
  {
    غیرعلامتی type = lookupType;
    منطقی is_ext = is_extension (c.جدول_لیبل_2);

    اگر (c.جدول_لیبل_2 != HB_OT_لیبل_GPOS && c.جدول_لیبل_2 != HB_OT_لیبل_GSUB)
      واپس Ok();

    اگر (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      واپس Ok();

    hb_سمتیہ_t<hb_pair_t<غیرعلامتی, hb_سمتیہ_t<غیرعلامتی>>> all_new_subtables;
    برائے (غیرعلامتی i = 0; i < sub_جدول.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      خودکار idx_res = c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &sub_جدول[i]);
      اگر (!idx_res.is_ok ()) جاری;
      غیرعلامتی subtable_اشاریہ = *idx_res;
      اگر (is_ext) {
        غیرعلامتی ext_subtable_اشاریہ = subtable_اشاریہ;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.گراف.object (ext_subtable_اشاریہ).سر_2;
        اگر (!extension || !extension->sanitize (c.گراف.vertices_[ext_subtable_اشاریہ]).is_ok ())
          جاری;

        TRY_تفویض (subtable_اشاریہ, extension->get_subtable_اشاریہ (c.گراف, ext_subtable_اشاریہ));
        type = extension->get_lookup_type ();
        اگر (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          جاری;
      }

      hb_سمتیہ_t<غیرعلامتی>* بانٹو_نتیجہ;
      اگر (c.بانٹو_subtables.has (subtable_اشاریہ, &بانٹو_نتیجہ))
      {
        اگر (بانٹو_نتیجہ->لمبائی == 0)
          جاری;
        all_new_subtables.push (hb_pair(i, *بانٹو_نتیجہ));
        TRY (گراف_نتیجہ_t<خالی>::from (all_new_subtables, تخصیص_FAILURE));
      }
      ورنہ
      {
        hb_سمتیہ_t<غیرعلامتی> new_sub_tables;

        اگر (c.جدول_لیبل_2 == HB_OT_لیبل_GPOS) {
          انتخاب (type)
          {
          حالت 2:
            {
              TRY_تفویض (new_sub_tables, بانٹو_subtable<PairPos> (c, subtable_اشاریہ));
              توڑو;
            }
          حالت 4:
            {
              TRY_تفویض (new_sub_tables, بانٹو_subtable<Mark_بنیاد_Pos> (c, subtable_اشاریہ));
              توڑو;
            }
          طےشدہ:
            توڑو;
          }
        } ورنہ اگر (c.جدول_لیبل_2 == HB_OT_لیبل_GSUB) {
          انتخاب (type)
          {
          حالت 4:
            {
              TRY_تفویض (new_sub_tables, بانٹو_subtable<گراف::LigatureSubst> (c, subtable_اشاریہ));
              توڑو;
            }
          طےشدہ:
            توڑو;
          }
        }

        c.بانٹو_subtables.سیٹ (subtable_اشاریہ, new_sub_tables);
        TRY (گراف_نتیجہ_t<خالی>::from (c.بانٹو_subtables, تخصیص_FAILURE));
        اگر (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (گراف_نتیجہ_t<خالی>::from (all_new_subtables, تخصیص_FAILURE));
        }
      }
    }

    اگر (all_new_subtables) {
      TRY (جوڑو_sub_tables (c, this_اشاریہ, type, all_new_subtables));
    }

    واپس Ok();
  }

  سانچہ<قسم_نام T>
  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtable (gsubgpos_گراف_context_t& c,
                                                        غیرعلامتی objidx)
  {
    T* sub_جدول_2 = (T*) c.گراف.object (objidx).سر_2;
    اگر (!sub_جدول_2 || !sub_جدول_2->sanitize (c.گراف.vertices_[objidx]).is_ok ())
      واپس Ok(hb_سمتیہ_t<غیرعلامتی> ());

    واپس sub_جدول_2->بانٹو_subtables (c, objidx);
  }

  گراف_نتیجہ_t<خالی> جوڑو_sub_tables (gsubgpos_گراف_context_t& c,
                                       غیرعلامتی this_اشاریہ,
                                       غیرعلامتی type,
                                       ثابت hb_سمتیہ_t<hb_pair_t<غیرعلامتی, hb_سمتیہ_t<غیرعلامتی>>>& subtable_ids)
  {
    منطقی is_ext = is_extension (c.جدول_لیبل_2);
    خودکار* v = &c.گراف.vertices_[this_اشاریہ];
    fix_existing_subtable_links (c, this_اشاریہ, subtable_ids);

    غیرعلامتی new_subtable_گنتی = 0;
    برائے (ثابت خودکار& p : subtable_ids)
      new_subtable_گنتی += p.second.لمبائی;

    size_t new_سائز = v->جدول_سائز ()
                      + new_subtable_گنتی * OT::Offset16::static_سائز;
    حرف* بفر = (حرف*) hb_calloc (1, new_سائز);
    اگر (unlikely (!بفر)) واپس Err(تخصیص_FAILURE);
    خودکار res = c.جوڑو_بفر (بفر);
    اگر (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      واپس res;
    }
    hb_memcpy (بفر, v->obj ().سر_2, v->جدول_سائز());

    اگر (use_mark_filtering_سیٹ ())
      hb_memcpy (بفر + new_سائز - 2, v->obj ().دم - 2, 2);

    v->سیٹ_بفر (بفر, بفر + new_سائز);

    Lookup* new_lookup = (Lookup*) بفر;

    غیرعلامتی سرکاؤ = 0;
    new_lookup->sub_جدول.len = sub_جدول.len + new_subtable_گنتی;
    برائے (ثابت خودکار& p : subtable_ids)
    {
      غیرعلامتی انحراف_اشاریہ = p.پہلا + سرکاؤ + 1;
      سرکاؤ += p.second.لمبائی;

      برائے (غیرعلامتی subtable_id : p.second)
      {
        اگر (is_ext)
        {
          TRY_تفویض (غیرعلامتی ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.گراف.vertices_[subtable_id].جوڑو_والد (ext_id, جھوٹ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.گراف.vertices_[this_اشاریہ];
        }

        TRY(v->جوڑو_real_link(2, subtable_id,
          (حرف*) &new_lookup->sub_جدول[انحراف_اشاریہ++] - (حرف*) new_lookup));
        TRY(c.گراف.vertices_[subtable_id].جوڑو_والد (this_اشاریہ, جھوٹ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->ترتیب_دو_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.سیٹ (this_اشاریہ, new_lookup);
    TRY (گراف_نتیجہ_t<خالی>::from (c.lookups, تخصیص_FAILURE));
    واپس Ok();
  }

  خالی fix_existing_subtable_links (gsubgpos_گراف_context_t& c,
                                    غیرعلامتی this_اشاریہ,
                                    ثابت hb_سمتیہ_t<hb_pair_t<غیرعلامتی, hb_سمتیہ_t<غیرعلامتی>>>& subtable_ids)
  {
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    غیرعلامتی سرکاؤ = 0;
    برائے (ثابت خودکار& p : subtable_ids)
    {
      غیرعلامتی داخل_کرو_اشاریہ = p.پہلا + سرکاؤ;
      غیرعلامتی pos_انحراف = p.second.لمبائی * OT::Offset16::static_سائز;
      غیرعلامتی داخل_کرو_انحراف = Lookup::min_سائز + داخل_کرو_اشاریہ * OT::Offset16::static_سائز;
      سرکاؤ += p.second.لمبائی;

      برائے (خودکار& l : v.all_links_writer ())
      {
        اگر (l.position > داخل_کرو_انحراف) l.position += pos_انحراف;
      }
    }
  }

  گراف_نتیجہ_t<غیرعلامتی> create_extension_subtable (gsubgpos_گراف_context_t& c,
                                                      غیرعلامتی subtable_اشاریہ,
                                                      غیرعلامتی type)
  {
    غیرعلامتی extension_سائز = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_سائز;

    TRY_تفویض (غیرعلامتی ext_اشاریہ, c.create_گرہ (extension_سائز));

    خودکار& ext_راس = c.گراف.vertices_[ext_اشاریہ];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_راس.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_راس.جوڑو_real_link (4, subtable_اشاریہ, 4));

    واپس Ok(ext_اشاریہ);
  }

  گراف_نتیجہ_t<خالی> make_subtable_extension (gsubgpos_گراف_context_t& c,
                                                غیرعلامتی lookup_اشاریہ,
                                                غیرعلامتی subtable_اشاریہ)
  {
    غیرعلامتی type = lookupType;
    غیرعلامتی ext_اشاریہ = HB_گراف_INVALID;
    غیرعلامتی* existing_ext_اشاریہ = خالی_اشاریہ;
    اگر (c.subtable_مقصد_extension.has(subtable_اشاریہ, &existing_ext_اشاریہ)) {
      ext_اشاریہ = *existing_ext_اشاریہ;
    } ورنہ {
      TRY_تفویض (ext_اشاریہ, create_extension_subtable(c, subtable_اشاریہ, type));
      c.subtable_مقصد_extension.سیٹ(subtable_اشاریہ, ext_اشاریہ);
      TRY (گراف_نتیجہ_t<خالی>::from (c.subtable_مقصد_extension, تخصیص_FAILURE));
    }

    خودکار& subtable_راس = c.گراف.vertices_[subtable_اشاریہ];
    خودکار& lookup_راس = c.گراف.vertices_[lookup_اشاریہ];
    غیرعلامتی replaced = 0;
    برائے (خودکار& l : lookup_راس.real_links_writer ())
    {
      اگر (l.objidx == subtable_اشاریہ) {
        // Change lookup to point at the extension.
        l.objidx = ext_اشاریہ;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    خودکار& ext_راس = c.گراف.vertices_[ext_اشاریہ];
    برائے (غیرعلامتی i = 0; i < replaced; i++)
    {
      subtable_راس.remove_والد (lookup_اشاریہ);
      TRY(ext_راس.جوڑو_والد (lookup_اشاریہ, جھوٹ));
    }
    اگر (!existing_ext_اشاریہ)
      TRY(subtable_راس.جوڑو_والد (ext_اشاریہ, جھوٹ));

    واپس Ok();
  }

 نجی:
  منطقی is_supported_gsub_type(غیرعلامتی type, gsubgpos_گراف_context_t& c) ثابت {
    واپس (c.جدول_لیبل_2 == HB_OT_لیبل_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_جدول::Type::Ligature
    );
  }

  منطقی is_supported_gpos_type(غیرعلامتی type, gsubgpos_گراف_context_t& c) ثابت {
   واپس (c.جدول_لیبل_2 == HB_OT_لیبل_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_جدول::Type::Mark_بنیاد
    );
  }

  غیرعلامتی extension_type (hb_لیبل_t جدول_لیبل_2) ثابت
  {
    انتخاب (جدول_لیبل_2)
    {
    حالت HB_OT_لیبل_GPOS: واپس 9;
    حالت HB_OT_لیبل_GSUB: واپس 7;
    طےشدہ: واپس 0;
    }
  }
};

سانچہ <قسم_نام T>
ساخت LookupList : عوامی OT::LookupList<T>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < OT::LookupList<T>::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (راس_len < OT::LookupList<T>::شے_سائز * یہ->len)) واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }
};

ساخت GSTAR : عوامی OT::GSUBGPOS
{
  ساکن گراف_نتیجہ_t<ثابت GSTAR*> گراف_مقصد_gstar (گراف_t& گراف)
  {
    ثابت خودکار& r = گراف.جڑ ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().سر_2;
    اگر (unlikely (!gstar)) واپس Err(INVALID_دلیل);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    واپس gstar;
  }

  ثابت خالی* get_lookup_list_میدان_انحراف () ثابت
  {
    واپس OT::GSUBGPOS::get_lookup_list_میدان_انحراف ();
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t len = راس.جدول_سائز ();
    اگر (unlikely (len < OT::GSUBGPOS::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (نسخہ.major != 1)) واپس Err(SANITIZE_FAILURE);
    اگر (unlikely (len < get_سائز ())) واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> ڈھونڈو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<غیرعلامتی, Lookup*>& lookups /*برآمد*/) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && lookupList2)
      واپس ڈھونڈو_lookups<MediumTypes> (گراف, lookups);
#اگر_ختم
    TRY(ڈھونڈو_lookups<SmallTypes> (گراف, lookups));
    واپس Ok();
  }

  گراف_نتیجہ_t<غیرعلامتی> get_lookup_list_اشاریہ (گراف_t& گراف) ثابت
  {
    ثابت خالی* انحراف = get_lookup_list_میدان_انحراف ();
    اگر (unlikely (!انحراف)) واپس Err(INVALID_دلیل);
    واپس گراف.اشاریہ_for_انحراف (گراف.جڑ_idx (), انحراف);
  }

  سانچہ<قسم_نام Types>
  گراف_نتیجہ_t<خالی> ڈھونڈو_lookups (گراف_t& گراف,
                                     hb_hashmap_t<غیرعلامتی, Lookup*>& lookups /*برآمد*/) ثابت
  {
    TRY_تفویض (غیرعلامتی lookup_list_idx, get_lookup_list_اشاریہ (گراف));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) گراف.object (lookup_list_idx).سر_2;
    اگر (unlikely (!lookupList)) واپس Err(INVALID_دلیل);
    TRY(lookupList->sanitize (گراف.vertices_[lookup_list_idx]));

    برائے (غیرعلامتی i = 0; i < lookupList->len; i++)
    {
      TRY_تفویض (غیرعلامتی lookup_idx, گراف.اشاریہ_for_انحراف (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) گراف.object (lookup_idx).سر_2;
      اگر (!lookup || !lookup->sanitize (گراف.vertices_[lookup_idx]).is_ok ()) جاری;
      lookups.سیٹ (lookup_idx, lookup);
    }
    واپس Ok();
  }
};




}

#اگر_ختم  /* GRAPH_GSUBGPOS_GRAPH_HH */
