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

#جے_بنا_تعریف HB_DRAW_HH
#تعریف_کرو HB_DRAW_HH

#ضمن "hb.hh"


/*
 * hb_draw_funcs_t
 */

#تعریف_کرو HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (سطر_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_منزل) \
  HB_DRAW_FUNC_IMPLEMENT (close_رستہ) \
  HB_DRAW_FUNC_IMPLEMENT (سیٹ_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_بچیا) \
  /* ^--- Add new callbacks here */

بنية hb_draw_funcs_t
{
  hb_object_سرخی_t سرخی;

  بنية {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_DRAW_FUNC_IMPLEMENT
  } func;

  بنية {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) فراغ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_DRAW_FUNC_IMPLEMENT
  } *user_انکڑے;

  بنية {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  فراغ emit_move_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
		     عائم منزل_x, عائم منزل_y)
  { func.move_منزل (هذا, draw_انکڑے, &st,
		  منزل_x, منزل_y,
		  !user_انکڑے ? خالی_سوچک : user_انکڑے->move_منزل); }
  فراغ emit_سطر_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
		     عائم منزل_x, عائم منزل_y)
  { func.سطر_منزل (هذا, draw_انکڑے, &st,
		  منزل_x, منزل_y,
		  !user_انکڑے ? خالی_سوچک : user_انکڑے->سطر_منزل); }
  فراغ emit_quadratic_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
			  عائم control_x, عائم control_y,
			  عائم منزل_x, عائم منزل_y)
  { func.quadratic_منزل (هذا, draw_انکڑے, &st,
		       control_x, control_y,
		       منزل_x, منزل_y,
		       !user_انکڑے ? خالی_سوچک : user_انکڑے->quadratic_منزل); }
  فراغ emit_cubic_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
		      عائم control1_x, عائم control1_y,
		      عائم control2_x, عائم control2_y,
		      عائم منزل_x, عائم منزل_y)
  { func.cubic_منزل (هذا, draw_انکڑے, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   منزل_x, منزل_y,
		   !user_انکڑے ? خالی_سوچک : user_انکڑے->cubic_منزل); }
  فراغ emit_close_رستہ (فراغ *draw_انکڑے, hb_draw_state_t &st)
  { func.close_رستہ (هذا, draw_انکڑے, &st,
		     !user_انکڑے ? خالی_سوچک : user_انکڑے->close_رستہ); }

  منطقي سیٹ_budget (فراغ *draw_انکڑے, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.سیٹ_budget (هذا, draw_انکڑے, budget,
			    !user_انکڑے ? خالی_سوچک : user_انکڑے->سیٹ_budget);
  }
  int64_t get_budget (فراغ *draw_انکڑے)
  { أرجع func.get_budget (هذا, draw_انکڑے,
			    !user_انکڑے ? خالی_سوچک : user_انکڑے->get_budget); }
  int64_t *get_budget_بچیا_ptr (فراغ *draw_انکڑے)
  { أرجع func.get_budget_بچیا (هذا, draw_انکڑے,
				      !user_انکڑے ? خالی_سوچک : user_انکڑے->get_budget_بچیا); }


  فراغ
  HB_ALWAYS_INLINE
  move_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
	   عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (st.رستہ_open)) close_رستہ (draw_انکڑے, st);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  سطر_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
	   عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.رستہ_open)) شروع_رستہ (draw_انکڑے, st);

    emit_سطر_منزل (draw_انکڑے, st, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  quadratic_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
		عائم control_x, عائم control_y,
		عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.رستہ_open)) شروع_رستہ (draw_انکڑے, st);

    emit_quadratic_منزل (draw_انکڑے, st, control_x, control_y, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  cubic_منزل (فراغ *draw_انکڑے, hb_draw_state_t &st,
	    عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم منزل_x, عائم منزل_y)
  {
    إذا (unlikely (!st.رستہ_open)) شروع_رستہ (draw_انکڑے, st);

    emit_cubic_منزل (draw_انکڑے, st, control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y);

    st.current_x = منزل_x;
    st.current_y = منزل_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  close_رستہ (فراغ *draw_انکڑے, hb_draw_state_t &st)
  {
    إذا (likely (st.رستہ_open))
    {
      إذا ((st.رستہ_شروع_x != st.current_x) || (st.رستہ_شروع_y != st.current_y))
	emit_سطر_منزل (draw_انکڑے, st, st.رستہ_شروع_x, st.رستہ_شروع_y);
      emit_close_رستہ (draw_انکڑے, st);
    }
    st.رستہ_open = خطأ;
    st.رستہ_شروع_x = st.current_x = st.رستہ_شروع_y = st.current_y = 0;
  }

  محمي:

  فراغ شروع_رستہ (فراغ *draw_انکڑے, hb_draw_state_t &st)
  {
    أكد (!st.رستہ_open);
    emit_move_منزل (draw_انکڑے, st, st.current_x, st.current_y);
    st.رستہ_open = سچ;
    st.رستہ_شروع_x = st.current_x;
    st.رستہ_شروع_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

بنية hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, فراغ *draw_انکڑے_2)
    : funcs {funcs_}, draw_انکڑے {draw_انکڑے_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_بچیا_ptr (draw_انکڑے)}
  {
    إذا (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_رستہ (); }

  HB_ALWAYS_INLINE
  فراغ move_منزل (عائم منزل_x, عائم منزل_y)
  {
    funcs->move_منزل (draw_انکڑے, st,
		    منزل_x, منزل_y);
  }
  HB_ALWAYS_INLINE
  فراغ سطر_منزل (عائم منزل_x, عائم منزل_y)
  {
    funcs->سطر_منزل (draw_انکڑے, st,
		    منزل_x, منزل_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  quadratic_منزل (عائم control_x, عائم control_y,
		عائم منزل_x, عائم منزل_y)
  {
    funcs->quadratic_منزل (draw_انکڑے, st,
			 control_x, control_y,
			 منزل_x, منزل_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  cubic_منزل (عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم منزل_x, عائم منزل_y)
  {
    funcs->cubic_منزل (draw_انکڑے, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     منزل_x, منزل_y);
  }
  HB_ALWAYS_INLINE
  فراغ close_رستہ ()
  {
    funcs->close_رستہ (draw_انکڑے, st);
  }

  int64_t &get_budget () { أرجع *budget; }

  عام:
  hb_draw_funcs_t *funcs;
  فراغ *draw_انکڑے;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_اندرونی hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#جے_انت /* HB_DRAW_HH */
