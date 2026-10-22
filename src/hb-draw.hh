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

#ifndef HB_DRAW_HH
#define HB_DRAW_HH

#അന്തര്ഭാവ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (വരി_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ഘടന hb_draw_funcs_t
{
  hb_object_header_t header;

  ഘടന {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  ഘടന {
#define HB_DRAW_FUNC_IMPLEMENT(name) ശൂന്യം *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  ഘടന {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  ശൂന്യം emit_move_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
		     ദശാംശം to_x, ദശാംശം to_y)
  { func.move_to (ഇത്, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  ശൂന്യം emit_വരി_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
		     ദശാംശം to_x, ദശാംശം to_y)
  { func.വരി_to (ഇത്, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->വരി_to); }
  ശൂന്യം emit_quadratic_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
			  ദശാംശം control_x, ദശാംശം control_y,
			  ദശാംശം to_x, ദശാംശം to_y)
  { func.quadratic_to (ഇത്, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  ശൂന്യം emit_cubic_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
		      ദശാംശം control1_x, ദശാംശം control1_y,
		      ദശാംശം control2_x, ദശാംശം control2_y,
		      ദശാംശം to_x, ദശാംശം to_y)
  { func.cubic_to (ഇത്, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  ശൂന്യം emit_close_path (ശൂന്യം *draw_data, hb_draw_state_t &st)
  { func.close_path (ഇത്, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  തർക്കം set_budget (ശൂന്യം *draw_data, int64_t budget)
  {
    എങ്കിൽ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    തിരികെ func.set_budget (ഇത്, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (ശൂന്യം *draw_data)
  { തിരികെ func.get_budget (ഇത്, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (ശൂന്യം *draw_data)
  { തിരികെ func.get_budget_remaining (ഇത്, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  ശൂന്യം
  HB_ALWAYS_INLINE
  move_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
	   ദശാംശം to_x, ദശാംശം to_y)
  {
    എങ്കിൽ (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ശൂന്യം
  HB_ALWAYS_INLINE
  വരി_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
	   ദശാംശം to_x, ദശാംശം to_y)
  {
    എങ്കിൽ (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_വരി_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ശൂന്യം
  HB_ALWAYS_INLINE
  quadratic_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
		ദശാംശം control_x, ദശാംശം control_y,
		ദശാംശം to_x, ദശാംശം to_y)
  {
    എങ്കിൽ (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ശൂന്യം
  HB_ALWAYS_INLINE
  cubic_to (ശൂന്യം *draw_data, hb_draw_state_t &st,
	    ദശാംശം control1_x, ദശാംശം control1_y,
	    ദശാംശം control2_x, ദശാംശം control2_y,
	    ദശാംശം to_x, ദശാംശം to_y)
  {
    എങ്കിൽ (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ശൂന്യം
  HB_ALWAYS_INLINE
  close_path (ശൂന്യം *draw_data, hb_draw_state_t &st)
  {
    എങ്കിൽ (likely (st.path_open))
    {
      എങ്കിൽ ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_വരി_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = അസത്യം;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  സംരക്ഷിത:

  ശൂന്യം start_path (ശൂന്യം *draw_data, hb_draw_state_t &st)
  {
    പ്രതിജ്ഞാ (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = സത്യം;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

ഘടന hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, ശൂന്യം *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    എങ്കിൽ (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  ശൂന്യം move_to (ദശാംശം to_x, ദശാംശം to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  ശൂന്യം വരി_to (ദശാംശം to_x, ദശാംശം to_y)
  {
    funcs->വരി_to (draw_data, st,
		    to_x, to_y);
  }
  ശൂന്യം
  HB_ALWAYS_INLINE
  quadratic_to (ദശാംശം control_x, ദശാംശം control_y,
		ദശാംശം to_x, ദശാംശം to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  ശൂന്യം
  HB_ALWAYS_INLINE
  cubic_to (ദശാംശം control1_x, ദശാംശം control1_y,
	    ദശാംശം control2_x, ദശാംശം control2_y,
	    ദശാംശം to_x, ദശാംശം to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  ശൂന്യം close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { തിരികെ *budget; }

  പൊതു:
  hb_draw_funcs_t *funcs;
  ശൂന്യം *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
