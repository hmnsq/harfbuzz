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

#که_نه_تعریف_شوی HB_DRAW_HH
#تعریف_کړه HB_DRAW_HH

#ضمن "hb.hh"


/*
 * hb_draw_funcs_t
 */

#تعریف_کړه HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (کرښه_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (close_لار) \
  HB_DRAW_FUNC_IMPLEMENT (سېټ_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_پاتې_شوی) \
  /* ^--- Add new callbacks here */

بنية hb_draw_funcs_t
{
  hb_object_سرلیک_t سرلیک;

  بنية {
#تعریف_کړه HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_لغوه HB_DRAW_FUNC_IMPLEMENT
  } func;

  بنية {
#تعریف_کړه HB_DRAW_FUNC_IMPLEMENT(name) فراغ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_لغوه HB_DRAW_FUNC_IMPLEMENT
  } *user_معلومات;

  بنية {
#تعریف_کړه HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_لغوه HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  فراغ emit_move_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
		     عائم منزل_x, عائم منزل_y)
  { func.move_منزل (هذا, draw_معلومات, &st,
		  منزل_x, منزل_y,
		  !user_معلومات ? تش_اشاره : user_معلومات->move_منزل); }
  فراغ emit_کرښه_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
		     عائم منزل_x, عائم منزل_y)
  { func.کرښه_منزل (هذا, draw_معلومات, &st,
		  منزل_x, منزل_y,
		  !user_معلومات ? تش_اشاره : user_معلومات->کرښه_منزل); }
  فراغ emit_quadratic_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
			  عائم control_x, عائم control_y,
			  عائم منزل_x, عائم منزل_y)
  { func.quadratic_منزل (هذا, draw_معلومات, &st,
		       control_x, control_y,
		       منزل_x, منزل_y,
		       !user_معلومات ? تش_اشاره : user_معلومات->quadratic_منزل); }
  فراغ emit_cubic_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
		      عائم control1_x, عائم control1_y,
		      عائم control2_x, عائم control2_y,
		      عائم منزل_x, عائم منزل_y)
  { func.cubic_منزل (هذا, draw_معلومات, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   منزل_x, منزل_y,
		   !user_معلومات ? تش_اشاره : user_معلومات->cubic_منزل); }
  فراغ emit_close_لار (فراغ *draw_معلومات, hb_draw_state_t &st)
  { func.close_لار (هذا, draw_معلومات, &st,
		     !user_معلومات ? تش_اشاره : user_معلومات->close_لار); }

  منطقي سېټ_budget (فراغ *draw_معلومات, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.سېټ_budget (هذا, draw_معلومات, budget,
			    !user_معلومات ? تش_اشاره : user_معلومات->سېټ_budget);
  }
  int64_t get_budget (فراغ *draw_معلومات)
  { أرجع func.get_budget (هذا, draw_معلومات,
			    !user_معلومات ? تش_اشاره : user_معلومات->get_budget); }
  int64_t *get_budget_پاتې_شوی_ptr (فراغ *draw_معلومات)
  { أرجع func.get_budget_پاتې_شوی (هذا, draw_معلومات,
				      !user_معلومات ? تش_اشاره : user_معلومات->get_budget_پاتې_شوی); }


  فراغ
  HB_ALWAYS_INLINE
  move_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
	   عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (st.لار_open)) close_لار (draw_معلومات, st);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  کرښه_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
	   عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.لار_open)) پیل_لار (draw_معلومات, st);

    emit_کرښه_منزل (draw_معلومات, st, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  quadratic_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
		عائم control_x, عائم control_y,
		عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.لار_open)) پیل_لار (draw_معلومات, st);

    emit_quadratic_منزل (draw_معلومات, st, control_x, control_y, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  cubic_منزل (فراغ *draw_معلومات, hb_draw_state_t &st,
	    عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.لار_open)) پیل_لار (draw_معلومات, st);

    emit_cubic_منزل (draw_معلومات, st, control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  close_لار (فراغ *draw_معلومات, hb_draw_state_t &st)
  {
    إذا (likely (st.لار_open))
    {
      إذا ((st.لار_پیل_x != st.current_x) || (st.لار_پیل_y != st.current_y))
	emit_کرښه_منزل (draw_معلومات, st, st.لار_پیل_x, st.لار_پیل_y);
      emit_close_لار (draw_معلومات, st);
    }
    st.لار_open = خطأ;
    st.لار_پیل_x = st.current_x = st.لار_پیل_y = st.current_y = 0;
  }

  محمي:

  فراغ پیل_لار (فراغ *draw_معلومات, hb_draw_state_t &st)
  {
    أكد (!st.لار_open);
    emit_move_منزل (draw_معلومات, st, st.current_x, st.current_y);
    st.لار_open = ریښتیا;
    st.لار_پیل_x = st.current_x;
    st.لار_پیل_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

بنية hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, فراغ *draw_معلومات_2)
    : funcs {funcs_}, draw_معلومات {draw_معلومات_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_پاتې_شوی_ptr (draw_معلومات)}
  {
    إذا (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_لار (); }

  HB_ALWAYS_INLINE
  فراغ move_منزل (عائم منزل_x, عائم منزل_y)
  {
    funcs->move_منزل (draw_معلومات, st,
		    منزل_x, منزل_y);
  }
  HB_ALWAYS_INLINE
  فراغ کرښه_منزل (عائم منزل_x, عائم منزل_y)
  {
    funcs->کرښه_منزل (draw_معلومات, st,
		    منزل_x, منزل_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  quadratic_منزل (عائم control_x, عائم control_y,
		عائم منزل_x, عائم منزل_y)
  {
    funcs->quadratic_منزل (draw_معلومات, st,
			 control_x, control_y,
			 منزل_x, منزل_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  cubic_منزل (عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم منزل_x, عائم منزل_y)
  {
    funcs->cubic_منزل (draw_معلومات, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     منزل_x, منزل_y);
  }
  HB_ALWAYS_INLINE
  فراغ close_لار ()
  {
    funcs->close_لار (draw_معلومات, st);
  }

  int64_t &get_budget () { أرجع *budget; }

  عام:
  hb_draw_funcs_t *funcs;
  فراغ *draw_معلومات;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_دننی hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#د_که_پای /* HB_DRAW_HH */
