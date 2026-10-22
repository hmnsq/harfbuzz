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

#اگر_غیر_تعریف_شدہ HB_DRAW_HH
#تعریف_کرو HB_DRAW_HH

#شامل "hb.hh"


/*
 * hb_draw_funcs_t
 */

#تعریف_کرو HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (سطر_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_مقصد) \
  HB_DRAW_FUNC_IMPLEMENT (close_راستہ) \
  HB_DRAW_FUNC_IMPLEMENT (سیٹ_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_بقایا) \
  /* ^--- Add new callbacks here */

ساخت hb_draw_funcs_t
{
  hb_object_سرخی_t سرخی;

  ساخت {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_DRAW_FUNC_IMPLEMENT
  } func;

  ساخت {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) خالی *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_DRAW_FUNC_IMPLEMENT
  } *user_ڈیٹا;

  ساخت {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  خالی emit_move_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
		     اعشاری مقصد_x, اعشاری مقصد_y)
  { func.move_مقصد (یہ, draw_ڈیٹا, &st,
		  مقصد_x, مقصد_y,
		  !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->move_مقصد); }
  خالی emit_سطر_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
		     اعشاری مقصد_x, اعشاری مقصد_y)
  { func.سطر_مقصد (یہ, draw_ڈیٹا, &st,
		  مقصد_x, مقصد_y,
		  !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->سطر_مقصد); }
  خالی emit_quadratic_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
			  اعشاری control_x, اعشاری control_y,
			  اعشاری مقصد_x, اعشاری مقصد_y)
  { func.quadratic_مقصد (یہ, draw_ڈیٹا, &st,
		       control_x, control_y,
		       مقصد_x, مقصد_y,
		       !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->quadratic_مقصد); }
  خالی emit_cubic_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
		      اعشاری control1_x, اعشاری control1_y,
		      اعشاری control2_x, اعشاری control2_y,
		      اعشاری مقصد_x, اعشاری مقصد_y)
  { func.cubic_مقصد (یہ, draw_ڈیٹا, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   مقصد_x, مقصد_y,
		   !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->cubic_مقصد); }
  خالی emit_close_راستہ (خالی *draw_ڈیٹا, hb_draw_state_t &st)
  { func.close_راستہ (یہ, draw_ڈیٹا, &st,
		     !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->close_راستہ); }

  منطقی سیٹ_budget (خالی *draw_ڈیٹا, int64_t budget)
  {
    اگر (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    واپس func.سیٹ_budget (یہ, draw_ڈیٹا, budget,
			    !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->سیٹ_budget);
  }
  int64_t get_budget (خالی *draw_ڈیٹا)
  { واپس func.get_budget (یہ, draw_ڈیٹا,
			    !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->get_budget); }
  int64_t *get_budget_بقایا_ptr (خالی *draw_ڈیٹا)
  { واپس func.get_budget_بقایا (یہ, draw_ڈیٹا,
				      !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->get_budget_بقایا); }


  خالی
  HB_ALWAYS_INLINE
  move_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
	   اعشاری مقصد_x, اعشاری مقصد_y)
  {
    اگر (unlikely (st.راستہ_open)) close_راستہ (draw_ڈیٹا, st);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  خالی
  HB_ALWAYS_INLINE
  سطر_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
	   اعشاری مقصد_x, اعشاری مقصد_y)
  {
    اگر (unlikely (!st.راستہ_open)) آغاز_راستہ (draw_ڈیٹا, st);

    emit_سطر_مقصد (draw_ڈیٹا, st, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  خالی
  HB_ALWAYS_INLINE
  quadratic_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
		اعشاری control_x, اعشاری control_y,
		اعشاری مقصد_x, اعشاری مقصد_y)
  {
    اگر (unlikely (!st.راستہ_open)) آغاز_راستہ (draw_ڈیٹا, st);

    emit_quadratic_مقصد (draw_ڈیٹا, st, control_x, control_y, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  خالی
  HB_ALWAYS_INLINE
  cubic_مقصد (خالی *draw_ڈیٹا, hb_draw_state_t &st,
	    اعشاری control1_x, اعشاری control1_y,
	    اعشاری control2_x, اعشاری control2_y,
	    اعشاری مقصد_x, اعشاری مقصد_y)
  {
    اگر (unlikely (!st.راستہ_open)) آغاز_راستہ (draw_ڈیٹا, st);

    emit_cubic_مقصد (draw_ڈیٹا, st, control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y);

    st.current_x = مقصد_x;
    st.current_y = مقصد_y;
  }

  خالی
  HB_ALWAYS_INLINE
  close_راستہ (خالی *draw_ڈیٹا, hb_draw_state_t &st)
  {
    اگر (likely (st.راستہ_open))
    {
      اگر ((st.راستہ_آغاز_x != st.current_x) || (st.راستہ_آغاز_y != st.current_y))
	emit_سطر_مقصد (draw_ڈیٹا, st, st.راستہ_آغاز_x, st.راستہ_آغاز_y);
      emit_close_راستہ (draw_ڈیٹا, st);
    }
    st.راستہ_open = جھوٹ;
    st.راستہ_آغاز_x = st.current_x = st.راستہ_آغاز_y = st.current_y = 0;
  }

  محفوظ:

  خالی آغاز_راستہ (خالی *draw_ڈیٹا, hb_draw_state_t &st)
  {
    أكد (!st.راستہ_open);
    emit_move_مقصد (draw_ڈیٹا, st, st.current_x, st.current_y);
    st.راستہ_open = سچ;
    st.راستہ_آغاز_x = st.current_x;
    st.راستہ_آغاز_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

ساخت hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, خالی *draw_ڈیٹا_2)
    : funcs {funcs_}, draw_ڈیٹا {draw_ڈیٹا_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_بقایا_ptr (draw_ڈیٹا)}
  {
    اگر (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_راستہ (); }

  HB_ALWAYS_INLINE
  خالی move_مقصد (اعشاری مقصد_x, اعشاری مقصد_y)
  {
    funcs->move_مقصد (draw_ڈیٹا, st,
		    مقصد_x, مقصد_y);
  }
  HB_ALWAYS_INLINE
  خالی سطر_مقصد (اعشاری مقصد_x, اعشاری مقصد_y)
  {
    funcs->سطر_مقصد (draw_ڈیٹا, st,
		    مقصد_x, مقصد_y);
  }
  خالی
  HB_ALWAYS_INLINE
  quadratic_مقصد (اعشاری control_x, اعشاری control_y,
		اعشاری مقصد_x, اعشاری مقصد_y)
  {
    funcs->quadratic_مقصد (draw_ڈیٹا, st,
			 control_x, control_y,
			 مقصد_x, مقصد_y);
  }
  خالی
  HB_ALWAYS_INLINE
  cubic_مقصد (اعشاری control1_x, اعشاری control1_y,
	    اعشاری control2_x, اعشاری control2_y,
	    اعشاری مقصد_x, اعشاری مقصد_y)
  {
    funcs->cubic_مقصد (draw_ڈیٹا, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     مقصد_x, مقصد_y);
  }
  HB_ALWAYS_INLINE
  خالی close_راستہ ()
  {
    funcs->close_راستہ (draw_ڈیٹا, st);
  }

  int64_t &get_budget () { واپس *budget; }

  عوامی:
  hb_draw_funcs_t *funcs;
  خالی *draw_ڈیٹا;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_اندرونی hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#اگر_ختم /* HB_DRAW_HH */
