/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */
#ifndef HB_OT_CFF_COMMON_HH
#define HB_OT_CFF_COMMON_HH

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-bimap.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-cff-interp-dict-common.hh"
#अन्तर्भाव "hb-subset-plan.hh"

नामाकाश CFF {

using नामाकाश OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
ढाँचा<typename Type, typename ...Ts>
स्थिर ओळीत स्थिरांक Type& StructAtOffsetOrNull (स्थिरांक शून्य *P, पूर्णांक offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  जर (!offset) परत Null (Type);

  स्थिरांक अक्षर *p = (स्थिरांक अक्षर *) P + offset;
  जर (!sc.check_point (p)) परत Null (Type);

  स्थिरांक Type &obj = *reinterpret_cast<स्थिरांक Type *> (p);
  जर (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) परत Null (Type);

  परत obj;
}


संरचना code_pair_t
{
  अचिन्हित code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<अचिन्हित अक्षर>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
संरचना Dict : UnsizedByteStr
{
  ढाँचा <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (हे);
    साठी (अचिन्हित पूर्णांक i = 0; i < dictval.get_मोजणी (); i++)
      जर (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (असत्य);

    return_trace (सत्य);
  }

  ढाँचा <typename T, typename V>
  स्थिर तार्किक serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    जर (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      परत असत्य;

    TRACE_SERIALIZE (हे);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), असत्य);
    जर (unlikely (!p)) return_trace (असत्य);
    जर (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (सत्य);
  }

  ढाँचा <typename V>
  स्थिर तार्किक serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { परत serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  ढाँचा <typename V>
  स्थिर तार्किक serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { परत serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  ढाँचा <typename T, पूर्णांक int_op>
  स्थिर तार्किक serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    जर (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) परत असत्य;
    c->add_link (ofs, link, whence);
    परत सत्य;
  }

  स्थिर तार्किक serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { परत serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  स्थिर तार्किक serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { परत serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

संरचना शीर्ष_Dict : Dict {};
संरचना FontDict : Dict {};
संरचना PrivateDict : Dict {};

संरचना table_info_t
{
  शून्य init () { offset = size = 0; link = 0; }

  अचिन्हित पूर्णांक    offset;
  अचिन्हित पूर्णांक    size;
  objidx_t	  link;
};

ढाँचा <typename मोजणी_3>
संरचना FDArray : CFFIndex<मोजणी_3>
{
  ढाँचा <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (हे);

    /* serialize INDEX data */
    hb_vector_t<अचिन्हित> sizes;
    जर (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    अक्षर *data_base = c->head;
    + it
    | hb_map ([&] (स्थिरांक hb_pair_t<स्थिरांक DICTVAL&, स्थिरांक INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		परत c->head - (स्थिरांक अक्षर*)dict;
	      })
    | hb_sink (sizes)
    ;
    अचिन्हित data_size = c->head - data_base;
    c->काढा_pack (असत्य);

    जर (unlikely (sizes.in_error ())) return_trace (असत्य);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<मोजणी_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
संरचना FDSelect0 {
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक glyph_मोजणी = HB_CODEPOINT_INVALID) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!(c->check_struct (हे))))
      return_trace (असत्य);
    hb_barrier ();
    जर (unlikely (!c->check_array (fds, glyph_मोजणी == HB_CODEPOINT_INVALID ?
                                     c->get_अंक_glyphs () : glyph_मोजणी)))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिरांक
  { परत fds[glyph]; }

  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिरांक
  { परत {fds[glyph], glyph + 1}; }

  माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  { परत hb_unsigned_mul_saturate (HBUINT8::static_size, अंक_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

ढाँचा <typename GID_TYPE, typename FD_TYPE>
संरचना FDSelect3_4_Range
{
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य * /*nullptr*/, अचिन्हित पूर्णांक fdcount) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  first < c->get_अंक_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

ढाँचा <typename GID_TYPE, typename FD_TYPE>
संरचना FDSelect3_4
{
  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_add_saturate ((माप_प्रकार) GID_TYPE::static_size * 2, ranges.get_size ()); }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक min_glyphs = HB_CODEPOINT_INVALID) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!(c->check_struct (हे) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (असत्य);

    साठी (अचिन्हित पूर्णांक i = 1; i < nRanges (); i++)
      जर (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (असत्य);

    जर (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_अंक_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_अंक_glyphs () : min_glyphs))))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  स्थिर पूर्णांक _cmp_range (स्थिरांक शून्य *किल्ली_3, स्थिरांक शून्य *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) किल्ली_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    जर (glyph < range[0].first) परत -1;
    जर (glyph < range[1].first) परत 0;
    परत +1;
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिरांक
  {
    स्वतः *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, आकार (ranges[0]), _cmp_range);
    परत range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिरांक
  {
    स्वतः *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, आकार (ranges[0]), _cmp_range);
    अचिन्हित fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    परत {fd, end};
  }

  GID_TYPE        &nRanges ()       { परत ranges.len; }
  GID_TYPE         nRanges () स्थिरांक { परत ranges.len; }
  GID_TYPE       &sentinel ()       { परत StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  स्थिरांक GID_TYPE &sentinel () स्थिरांक { परत StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

प्रकारव्याख्या FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
प्रकारव्याख्या FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

संरचना FDSelect
{
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक FDSelect &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित पूर्णांक size = src.get_size (अंक_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, असत्य);
    जर (unlikely (!dest)) return_trace (असत्य);
    hb_memcpy (dest, &src, size);
    return_trace (सत्य);
  }

  माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत hb_unsigned_add_saturate (format.static_size, u.format0.get_size (अंक_glyphs));
    स्थिती 3: hb_barrier (); परत hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    पूर्वनिर्धारित:परत 0;
    }
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिरांक
  {
    जर (हे == &Null (FDSelect)) परत 0;

    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_fd (glyph);
    स्थिती 3: hb_barrier (); परत u.format3.get_fd (glyph);
    पूर्वनिर्धारित:परत 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिरांक
  {
    जर (हे == &Null (FDSelect)) परत {0, 1};

    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_fd_range (glyph);
    स्थिती 3: hb_barrier (); परत u.format3.get_fd_range (glyph);
    पूर्वनिर्धारित:परत {0, 1};
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    निवड (format)
    {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  HBUINT8	format;
  संघ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (1);
};

ढाँचा <typename मोजणी_3>
संरचना Subrs : CFFIndex<मोजणी_3>
{
  प्रकारव्याख्या मोजणी_3 मोजणी_type;
  प्रकारव्याख्या CFFIndex<मोजणी_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
