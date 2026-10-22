/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_KERN_TABLE_HH
#define HB_OT_KERN_TABLE_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


فضاینام OT {


template <typename KernSubTableHeader>
ساختار KernSubTableFormat3
{
  صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
  {
    hb_array_t<ثابت FWORD> kern_مقدار = kern_مقدار_Z.as_array (kern_مقدار_شمار);
    hb_array_t<ثابت HBUINT8> چپ_Class = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (kern_مقدار).as_array (glyph_شمار_2);
    hb_array_t<ثابت HBUINT8> راست_Class = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (چپ_Class).as_array (glyph_شمار_2);
    hb_array_t<ثابت HBUINT8> kernIndex = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (راست_Class).as_array (چپ_Class_شمار * راست_Class_شمار);

    unsigned صحیح چپ_C = چپ_Class[left];
    unsigned صحیح راست_C = راست_Class[راست];
    اگر (unlikely (چپ_C >= چپ_Class_شمار || راست_C >= راست_Class_شمار))
      بازگشت 0;
    unsigned صحیح i = چپ_C * راست_Class_شمار + راست_C;
    بازگشت kern_مقدار[kernIndex[i]];
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    اگر (!c->plan->requested_kerning)
      بازگشت نادرست;

    اگر (header.coverage & header.Backwards)
      بازگشت نادرست;

    hb_kern_machine_t<KernSubTableFormat3> machine (*این, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  c->check_range (kern_مقدار_Z,
				  kern_مقدار_شمار * اندازه (FWORD) +
				  glyph_شمار_2 * 2 +
				  چپ_Class_شمار * راست_Class_شمار));
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &چپ_set, set_t &راست_set, unsigned رقم_glyphs) ثابت
  {
    اگر (likely (glyph_شمار_2))
    {
      چپ_set.add_range (0, رقم_glyphs - 1);
      راست_set.add_range (0, رقم_glyphs - 1);
    }
  }

  محافظتشده:
  KernSubTableHeader
		header;
  HBUINT16	glyph_شمار_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_مقدار_شمار;	/* The number of kerning values. */
  HBUINT8	چپ_Class_شمار;	/* The number of left-hand classes. */
  HBUINT8	راست_Class_شمار;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_مقدار_Z;	/* The kerning values.
				 * Length kernValueCount. */
#اگر 0
  UnsizedArrayOf<HBUINT8>
		چپ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		راست_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  عمومی:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_مقدار_Z);
};

template <typename KernSubTableHeader>
ساختار KernSubTable
{
  size_t get_size () ثابت { بازگشت u.header.length; }
  unsigned صحیح get_type () ثابت { بازگشت u.header.format; }

  صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
  {
    انتخاب (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالت 0: hb_barrier (); بازگشت u.format0.get_kerning (left, راست);
    پیشفرض:بازگشت 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحیح subtable_type = get_type ();
    TRACE_DISPATCH (این, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    حالت 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    حالت 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    پیشفرض:	return_trace (c->default_return_مقدار ());
    }
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &چپ_set, set_t &راست_set, unsigned رقم_glyphs) ثابت
  {
    unsigned صحیح subtable_type = get_type ();
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); u.format0.collect_glyphs (چپ_set, راست_set, رقم_glyphs); بازگشت;
#ifndef HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); u.format1.collect_glyphs (چپ_set, راست_set, رقم_glyphs); بازگشت;
#endif
    حالت 2:	hb_barrier (); u.format2.collect_glyphs (چپ_set, راست_set, رقم_glyphs); بازگشت;
#ifndef HB_NO_AAT_SHAPE
    حالت 3:	hb_barrier (); u.format3.collect_glyphs (چپ_set, راست_set, رقم_glyphs); بازگشت;
#endif
    پیشفرض:	بازگشت;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (این, u.header.length)))) return_trace (نادرست);

    return_trace (dispatch (c));
  }

  عمومی:
  اجتماع {
  KernSubTableHeader				header;
  AAT::KerxSubTableFormat0<KernSubTableHeader>	format0;
#ifndef HB_NO_AAT_SHAPE
  AAT::KerxSubTableFormat1<KernSubTableHeader>	format1;
#endif
  AAT::KerxSubTableFormat2<KernSubTableHeader>	format2;
#ifndef HB_NO_AAT_SHAPE
  KernSubTableFormat3<KernSubTableHeader>	format3;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


ساختار KernOTSubTableHeader
{
  ایستا constexpr منطقی apple = نادرست;
  تعریفنوع AAT::ObsoleteTypes Types;

  unsigned   tuple_شمار_2 () ثابت { بازگشت 0; }
  منطقی     is_horizontal () ثابت { بازگشت (coverage & Horizontal); }

  شمارش Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار KernOT : AAT::KerxTable<KernOT>
{
  friend ساختار AAT::KerxTable<KernOT>;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ایستا constexpr unsigned minVersion = 0u;

  تعریفنوع KernOTSubTableHeader SubTableHeader;
  تعریفنوع SubTableHeader::Types Types;
  تعریفنوع KernSubTable<SubTableHeader> SubTable;

  محافظتشده:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_شمار;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  عمومی:
  DEFINE_SIZE_MIN (4);
};


ساختار KernAATSubTableHeader
{
  ایستا constexpr منطقی apple = درست;
  تعریفنوع AAT::ObsoleteTypes Types;

  unsigned   tuple_شمار_2 () ثابت { بازگشت 0; }
  منطقی     is_horizontal () ثابت { بازگشت !(coverage & Vertical); }

  شمارش Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار KernAAT : AAT::KerxTable<KernAAT>
{
  friend ساختار AAT::KerxTable<KernAAT>;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ایستا constexpr unsigned minVersion = 0x00010000u;

  تعریفنوع KernAATSubTableHeader SubTableHeader;
  تعریفنوع SubTableHeader::Types Types;
  تعریفنوع KernSubTable<SubTableHeader> SubTable;

  محافظتشده:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_شمار;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  عمومی:
  DEFINE_SIZE_MIN (8);
};

ساختار kern
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  منطقی     has_data () ثابت { بازگشت u.version32.v; }
  unsigned get_type () ثابت { بازگشت u.major.v; }

  منطقی has_state_machine () ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); بازگشت u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); بازگشت u.aat.has_state_machine ();
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  منطقی has_cross_stream () ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); بازگشت u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); بازگشت u.aat.has_cross_stream ();
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  صحیح get_h_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); بازگشت u.ot.get_h_kerning (left, راست);
#ifndef HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); بازگشت u.aat.get_h_kerning (left, راست);
#endif
    پیشفرض:بازگشت 0;
    }
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_data_t &accel_data) ثابت
  { بازگشت dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحیح subtable_type = get_type ();
    TRACE_DISPATCH (این, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    پیشفرض:	return_trace (c->default_return_مقدار ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.version32.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned رقم_glyphs) ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); بازگشت u.ot.create_accelerator_data (رقم_glyphs);
#ifndef HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); بازگشت u.aat.create_accelerator_data (رقم_glyphs);
#endif
    پیشفرض:بازگشت AAT::kern_accelerator_data_t ();
    }
  }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      این->table = sc.reference_table<kern> (face);
      این->accel_data = این->table->create_accelerator_data (face->get_رقم_glyphs ());
    }
    ~accelerator_t ()
    {
      این->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { بازگشت table.get_blob (); }

    منطقی apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      بازگشت table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  محافظتشده:
  اجتماع {
  ساختار { HBUINT32 v; }	version32;
  ساختار { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_UNION (4, version32.v);
};

ساختار kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
