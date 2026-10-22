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

#ضمن "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (line_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

بنية hb_draw_funcs_t
{
  hb_object_header_t header;

  بنية {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  بنية {
#define HB_DRAW_FUNC_IMPLEMENT(name) فراغ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  بنية {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  فراغ emit_move_to (فراغ *draw_data, hb_draw_state_t &st,
		     عائم to_x, عائم to_y)
  { func.move_to (هذا, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  فراغ emit_line_to (فراغ *draw_data, hb_draw_state_t &st,
		     عائم to_x, عائم to_y)
  { func.line_to (هذا, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->line_to); }
  فراغ emit_quadratic_to (فراغ *draw_data, hb_draw_state_t &st,
			  عائم control_x, عائم control_y,
			  عائم to_x, عائم to_y)
  { func.quadratic_to (هذا, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  فراغ emit_cubic_to (فراغ *draw_data, hb_draw_state_t &st,
		      عائم control1_x, عائم control1_y,
		      عائم control2_x, عائم control2_y,
		      عائم to_x, عائم to_y)
  { func.cubic_to (هذا, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  فراغ emit_close_path (فراغ *draw_data, hb_draw_state_t &st)
  { func.close_path (هذا, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  منطقي set_budget (فراغ *draw_data, int64_t budget)
  {
    𐡄𐡍 (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.set_budget (هذا, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (فراغ *draw_data)
  { أرجع func.get_budget (هذا, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (فراغ *draw_data)
  { أرجع func.get_budget_remaining (هذا, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  فراغ
  HB_ALWAYS_INLINE
  move_to (فراغ *draw_data, hb_draw_state_t &st,
	   عائم to_x, عائم to_y)
  {
    𐡄𐡍 (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  line_to (فراغ *draw_data, hb_draw_state_t &st,
	   عائم to_x, عائم to_y)
  {
    𐡄𐡍 (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_line_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  quadratic_to (فراغ *draw_data, hb_draw_state_t &st,
		عائم control_x, عائم control_y,
		عائم to_x, عائم to_y)
  {
    𐡄𐡍 (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  cubic_to (فراغ *draw_data, hb_draw_state_t &st,
	    عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم to_x, عائم to_y)
  {
    𐡄𐡍 (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  فراغ
  HB_ALWAYS_INLINE
  close_path (فراغ *draw_data, hb_draw_state_t &st)
  {
    𐡄𐡍 (likely (st.path_open))
    {
      𐡄𐡍 ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_line_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = خطأ;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  محمي:

  فراغ start_path (فراغ *draw_data, hb_draw_state_t &st)
  {
    أكد (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = 𐡒𐡔𐡈;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

بنية hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, فراغ *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    𐡄𐡍 (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  فراغ move_to (عائم to_x, عائم to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  فراغ line_to (عائم to_x, عائم to_y)
  {
    funcs->line_to (draw_data, st,
		    to_x, to_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  quadratic_to (عائم control_x, عائم control_y,
		عائم to_x, عائم to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  فراغ
  HB_ALWAYS_INLINE
  cubic_to (عائم control1_x, عائم control1_y,
	    عائم control2_x, عائم control2_y,
	    عائم to_x, عائم to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  فراغ close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { أرجع *budget; }

  عام:
  hb_draw_funcs_t *funcs;
  فراغ *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
