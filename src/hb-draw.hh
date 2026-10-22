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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DRAW_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_ਮੰਜ਼ਿਲ) \
  HB_DRAW_FUNC_IMPLEMENT (ਸਤਰ_ਮੰਜ਼ਿਲ) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_ਮੰਜ਼ਿਲ) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_ਮੰਜ਼ਿਲ) \
  HB_DRAW_FUNC_IMPLEMENT (close_ਰਸਤਾ) \
  HB_DRAW_FUNC_IMPLEMENT (ਸੈੱਟ_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_ਬਚਿਆ) \
  /* ^--- Add new callbacks here */

ਬਣਤਰ hb_draw_funcs_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
  } func;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) ਖਾਲੀ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
  } *user_ਅੰਕੜੇ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  ਖਾਲੀ emit_move_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  { func.move_ਮੰਜ਼ਿਲ (ਇਹ, draw_ਅੰਕੜੇ, &st,
		  ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y,
		  !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->move_ਮੰਜ਼ਿਲ); }
  ਖਾਲੀ emit_ਸਤਰ_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  { func.ਸਤਰ_ਮੰਜ਼ਿਲ (ਇਹ, draw_ਅੰਕੜੇ, &st,
		  ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y,
		  !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਸਤਰ_ਮੰਜ਼ਿਲ); }
  ਖਾਲੀ emit_quadratic_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
			  ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  { func.quadratic_ਮੰਜ਼ਿਲ (ਇਹ, draw_ਅੰਕੜੇ, &st,
		       control_x, control_y,
		       ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y,
		       !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->quadratic_ਮੰਜ਼ਿਲ); }
  ਖਾਲੀ emit_cubic_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
		      ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
		      ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
		      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  { func.cubic_ਮੰਜ਼ਿਲ (ਇਹ, draw_ਅੰਕੜੇ, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y,
		   !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->cubic_ਮੰਜ਼ਿਲ); }
  ਖਾਲੀ emit_close_ਰਸਤਾ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st)
  { func.close_ਰਸਤਾ (ਇਹ, draw_ਅੰਕੜੇ, &st,
		     !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->close_ਰਸਤਾ); }

  ਤਾਰਕਿਕ ਸੈੱਟ_budget (ਖਾਲੀ *draw_ਅੰਕੜੇ, int64_t budget)
  {
    ਜੇ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ਵਾਪਸ func.ਸੈੱਟ_budget (ਇਹ, draw_ਅੰਕੜੇ, budget,
			    !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਸੈੱਟ_budget);
  }
  int64_t get_budget (ਖਾਲੀ *draw_ਅੰਕੜੇ)
  { ਵਾਪਸ func.get_budget (ਇਹ, draw_ਅੰਕੜੇ,
			    !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->get_budget); }
  int64_t *get_budget_ਬਚਿਆ_ptr (ਖਾਲੀ *draw_ਅੰਕੜੇ)
  { ਵਾਪਸ func.get_budget_ਬਚਿਆ (ਇਹ, draw_ਅੰਕੜੇ,
				      !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->get_budget_ਬਚਿਆ); }


  ਖਾਲੀ
  HB_ALWAYS_INLINE
  move_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
	   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    ਜੇ (unlikely (st.ਰਸਤਾ_open)) close_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);

    st.current_x = ਮੰਜ਼ਿਲ_x;
    st.current_y = ਮੰਜ਼ਿਲ_y;
  }

  ਖਾਲੀ
  HB_ALWAYS_INLINE
  ਸਤਰ_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
	   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    ਜੇ (unlikely (!st.ਰਸਤਾ_open)) ਸ਼ੁਰੂ_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);

    emit_ਸਤਰ_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

    st.current_x = ਮੰਜ਼ਿਲ_x;
    st.current_y = ਮੰਜ਼ਿਲ_y;
  }

  ਖਾਲੀ
  HB_ALWAYS_INLINE
  quadratic_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
		ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
		ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    ਜੇ (unlikely (!st.ਰਸਤਾ_open)) ਸ਼ੁਰੂ_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);

    emit_quadratic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st, control_x, control_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

    st.current_x = ਮੰਜ਼ਿਲ_x;
    st.current_y = ਮੰਜ਼ਿਲ_y;
  }

  ਖਾਲੀ
  HB_ALWAYS_INLINE
  cubic_ਮੰਜ਼ਿਲ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st,
	    ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
	    ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
	    ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    ਜੇ (unlikely (!st.ਰਸਤਾ_open)) ਸ਼ੁਰੂ_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);

    emit_cubic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st, control1_x, control1_y, control2_x, control2_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

    st.current_x = ਮੰਜ਼ਿਲ_x;
    st.current_y = ਮੰਜ਼ਿਲ_y;
  }

  ਖਾਲੀ
  HB_ALWAYS_INLINE
  close_ਰਸਤਾ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st)
  {
    ਜੇ (likely (st.ਰਸਤਾ_open))
    {
      ਜੇ ((st.ਰਸਤਾ_ਸ਼ੁਰੂ_x != st.current_x) || (st.ਰਸਤਾ_ਸ਼ੁਰੂ_y != st.current_y))
	emit_ਸਤਰ_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st, st.ਰਸਤਾ_ਸ਼ੁਰੂ_x, st.ਰਸਤਾ_ਸ਼ੁਰੂ_y);
      emit_close_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);
    }
    st.ਰਸਤਾ_open = ਝੂਠ;
    st.ਰਸਤਾ_ਸ਼ੁਰੂ_x = st.current_x = st.ਰਸਤਾ_ਸ਼ੁਰੂ_y = st.current_y = 0;
  }

  ਸੁਰੱਖਿਅਤ:

  ਖਾਲੀ ਸ਼ੁਰੂ_ਰਸਤਾ (ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t &st)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (!st.ਰਸਤਾ_open);
    emit_move_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st, st.current_x, st.current_y);
    st.ਰਸਤਾ_open = ਸੱਚ;
    st.ਰਸਤਾ_ਸ਼ੁਰੂ_x = st.current_x;
    st.ਰਸਤਾ_ਸ਼ੁਰੂ_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

ਬਣਤਰ hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, ਖਾਲੀ *draw_ਅੰਕੜੇ_2)
    : funcs {funcs_}, draw_ਅੰਕੜੇ {draw_ਅੰਕੜੇ_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_ਬਚਿਆ_ptr (draw_ਅੰਕੜੇ)}
  {
    ਜੇ (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_ਰਸਤਾ (); }

  HB_ALWAYS_INLINE
  ਖਾਲੀ move_ਮੰਜ਼ਿਲ (ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    funcs->move_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st,
		    ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  }
  HB_ALWAYS_INLINE
  ਖਾਲੀ ਸਤਰ_ਮੰਜ਼ਿਲ (ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    funcs->ਸਤਰ_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st,
		    ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  }
  ਖਾਲੀ
  HB_ALWAYS_INLINE
  quadratic_ਮੰਜ਼ਿਲ (ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
		ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    funcs->quadratic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st,
			 control_x, control_y,
			 ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  }
  ਖਾਲੀ
  HB_ALWAYS_INLINE
  cubic_ਮੰਜ਼ਿਲ (ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
	    ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
	    ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
  {
    funcs->cubic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  }
  HB_ALWAYS_INLINE
  ਖਾਲੀ close_ਰਸਤਾ ()
  {
    funcs->close_ਰਸਤਾ (draw_ਅੰਕੜੇ, st);
  }

  int64_t &get_budget () { ਵਾਪਸ *budget; }

  ਜਨਤਕ:
  hb_draw_funcs_t *funcs;
  ਖਾਲੀ *draw_ਅੰਕੜੇ;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_ਅੰਦਰੂਨੀ hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#ਜੇ_ਅੰਤ /* HB_DRAW_HH */
