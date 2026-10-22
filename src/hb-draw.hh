/*
 * Copyright © 2020  Ebrahim Byagowi
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
 */

#اگر_تعریف_نشده HB_DRAW_HH
#تعریف_کن HB_DRAW_HH

#ضمن "hb.hh"


/*
 * hb_draw_funcs_t
 */

#تعریف_کن HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (سطر_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (close_مسیر) \
  HB_DRAW_FUNC_IMPLEMENT (مجموعه_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_مانده) \
  /* ^--- Add new callbacks here */

بنية hb_draw_funcs_t
{
  hb_object_سرآیند_t سرآیند;

  بنية {
#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT
  } func;

  بنية {
#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name) فراغ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT
  } *user_داده;

  بنية {
#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  فراغ emit_move_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
		     عائم مقصد_x, عائم مقصد_y)
  { func.move_مقصد (هذا, draw_داده, &st,
		  مقصد_x, مقصد_y,
		  !user_داده ? اشاره_تهی : user_داده->move_مقصد); }
  فراغ emit_سطر_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
		     عائم مقصد_x, عائم مقصد_y)
  { func.سطر_مقصد (هذا, draw_داده, &st,
		  مقصد_x, مقصد_y,
		  !user_داده ? اشاره_تهی : user_داده->سطر_مقصد); }
  فراغ emit_quadratic_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
			  عائم control_x, عائم control_y,
			  عائم مقصد_x, عائم مقصد_y)
  { func.quadratic_مقصد (هذا, draw_داده, &st,
		       control_x, control_y,
		       مقصد_x, مقصد_y,
		       !user_داده ? اشاره_تهی : user_داده->quadratic_مقصد); }
  فراغ emit_cubic_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
		      عائم control1_x, عائم control1_y,
		      عائم control2_x, عائم control2_y,
		      عائم مقصد_x, عائم مقصد_y)
  { func.cubic_مقصد (هذا, draw_داده, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   مقصد_x, مقصد_y,
		   !user_داده ? اشاره_تهی : user_داده->cubic_مقصد); }
  فراغ emit_close_مسیر (فراغ *draw_داده, hb_draw_state_t &st)
  { func.close_مسیر (هذا, draw_داده, &st,
		     !user_داده ? اشاره_تهی : user_داده->close_مسیر); }

  منطقي مجموعه_budget (فراغ *draw_داده, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.مجموعه_budget (هذا, draw_داده, budget,
			    !user_داده ? اشاره_تهی : user_داده->مجموعه_budget);
  }
  int64_t get_budget (فراغ *draw_داده)
  { أرجع func.get_budget (هذا, draw_داده,
			    !user_داده ? اشاره_تهی : user_داده->get_budget); }
  int64_t *get_budget_مانده_ptr (فراغ *draw_داده)
  { أرجع func.get_budget_مانده (هذا, draw_داده,
				      !user_داده ? اشاره_تهی : user_داده->get_budget_مانده); }


  فراغ
  HB_ALWAYS_INLINE
  move_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
	   عائم مقصد_x, عائم مقصد_y)
  {
    إذا (unlikely (st.مسیر_open)) close_مسیر (draw_داده, st);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  سطر_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
	   عائم مقصد_x, عائم مقصد_y)
  {
    إذا (unlikely (!st.مسیر_open)) شروع_مسیر (draw_داده, st);

    emit_سطر_مقصد (draw_داده, st, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  quadratic_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
		عائم control_x, عائم control_y,
		عائم مقصد_x, عائم مقصد_y)
  {
    إذا (unlikely (!st.مسیر_open)) شروع_مسیر (draw_داده, st);

    emit_quadratic_مقصد (draw_داده, st, control_x, control_y, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  cubic_مقصد (فراغ *draw_داده, hb_draw_state_t &st,
	    عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم مقصد_x, عائم مقصد_y)
  {
    إذا (unlikely (!st.مسیر_open)) شروع_مسیر (draw_داده, st);

    emit_cubic_مقصد (draw_داده, st, control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  close_مسیر (فراغ *draw_داده, hb_draw_state_t &st)
  {
    إذا (likely (st.مسیر_open))
    {
      إذا ((st.مسیر_شروع_x != st.current_x) || (st.مسیر_شروع_y != st.current_y))
	emit_سطر_مقصد (draw_داده, st, st.مسیر_شروع_x, st.مسیر_شروع_y);
      emit_close_مسیر (draw_داده, st);
    }
    st.مسیر_open = خطأ;
    st.مسیر_شروع_x = st.current_x = st.مسیر_شروع_y = st.current_y = 0;
  }

  محمي:

  فراغ شروع_مسیر (فراغ *draw_داده, hb_draw_state_t &st)
  {
    أكد (!st.مسیر_open);
    emit_move_مقصد (draw_داده, st, st.current_x, st.current_y);
    st.مسیر_open = درست;
    st.مسیر_شروع_x = st.current_x;
    st.مسیر_شروع_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

بنية hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, فراغ *draw_داده_2)
    : funcs {funcs_}, draw_داده {draw_داده_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_مانده_ptr (draw_داده)}
  {
    إذا (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_مسیر (); }

  HB_ALWAYS_INLINE
  فراغ move_مقصد (عائم مقصد_x, عائم مقصد_y)
  {
    funcs->move_مقصد (draw_داده, st,
		    مقصد_x, مقصد_y);
  }
  HB_ALWAYS_INLINE
  فراغ سطر_مقصد (عائم مقصد_x, عائم مقصد_y)
  {
    funcs->سطر_مقصد (draw_داده, st,
		    مقصد_x, مقصد_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  quadratic_مقصد (عائم control_x, عائم control_y,
		عائم مقصد_x, عائم مقصد_y)
  {
    funcs->quadratic_مقصد (draw_داده, st,
			 control_x, control_y,
			 مقصد_x, مقصد_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  cubic_مقصد (عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم مقصد_x, عائم مقصد_y)
  {
    funcs->cubic_مقصد (draw_داده, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     مقصد_x, مقصد_y);
  }
  HB_ALWAYS_INLINE
  فراغ close_مسیر ()
  {
    funcs->close_مسیر (draw_داده, st);
  }

  int64_t &get_budget () { أرجع *budget; }

  عام:
  hb_draw_funcs_t *funcs;
  فراغ *draw_داده;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_داخلی hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#پایان_اگر /* HB_DRAW_HH */
