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

#ଅନ୍ତର୍ଭାଵ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (ଧାଡ଼ି_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ସଂରଚନା hb_draw_funcs_t
{
  hb_object_header_t header;

  ସଂରଚନା {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  ସଂରଚନା {
#define HB_DRAW_FUNC_IMPLEMENT(name) ଶୂନ୍ୟ *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  ସଂରଚନା {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  ଶୂନ୍ୟ emit_move_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
		     ଦଶମିକ to_x, ଦଶମିକ to_y)
  { func.move_to (ଏହି, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  ଶୂନ୍ୟ emit_ଧାଡ଼ି_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
		     ଦଶମିକ to_x, ଦଶମିକ to_y)
  { func.ଧାଡ଼ି_to (ଏହି, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->ଧାଡ଼ି_to); }
  ଶୂନ୍ୟ emit_quadratic_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
			  ଦଶମିକ control_x, ଦଶମିକ control_y,
			  ଦଶମିକ to_x, ଦଶମିକ to_y)
  { func.quadratic_to (ଏହି, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  ଶୂନ୍ୟ emit_cubic_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
		      ଦଶମିକ control1_x, ଦଶମିକ control1_y,
		      ଦଶମିକ control2_x, ଦଶମିକ control2_y,
		      ଦଶମିକ to_x, ଦଶମିକ to_y)
  { func.cubic_to (ଏହି, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  ଶୂନ୍ୟ emit_close_path (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st)
  { func.close_path (ଏହି, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  ତାର୍କିକ set_budget (ଶୂନ୍ୟ *draw_data, int64_t budget)
  {
    ଯଦି (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ଫେରନ୍ତୁ func.set_budget (ଏହି, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (ଶୂନ୍ୟ *draw_data)
  { ଫେରନ୍ତୁ func.get_budget (ଏହି, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (ଶୂନ୍ୟ *draw_data)
  { ଫେରନ୍ତୁ func.get_budget_remaining (ଏହି, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  move_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
	   ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    ଯଦି (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  ଧାଡ଼ି_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
	   ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    ଯଦି (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_ଧାଡ଼ି_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  quadratic_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
		ଦଶମିକ control_x, ଦଶମିକ control_y,
		ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    ଯଦି (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  cubic_to (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st,
	    ଦଶମିକ control1_x, ଦଶମିକ control1_y,
	    ଦଶମିକ control2_x, ଦଶମିକ control2_y,
	    ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    ଯଦି (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  close_path (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st)
  {
    ଯଦି (likely (st.path_open))
    {
      ଯଦି ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_ଧାଡ଼ି_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = ମିଥ୍ୟା;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  ସୁରକ୍ଷିତ:

  ଶୂନ୍ୟ start_path (ଶୂନ୍ୟ *draw_data, hb_draw_state_t &st)
  {
    ପ୍ରତିଜ୍ଞା (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = ସତ୍ୟ;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

ସଂରଚନା hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, ଶୂନ୍ୟ *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    ଯଦି (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ move_to (ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ ଧାଡ଼ି_to (ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    funcs->ଧାଡ଼ି_to (draw_data, st,
		    to_x, to_y);
  }
  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  quadratic_to (ଦଶମିକ control_x, ଦଶମିକ control_y,
		ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  ଶୂନ୍ୟ
  HB_ALWAYS_INLINE
  cubic_to (ଦଶମିକ control1_x, ଦଶମିକ control1_y,
	    ଦଶମିକ control2_x, ଦଶମିକ control2_y,
	    ଦଶମିକ to_x, ଦଶମିକ to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { ଫେରନ୍ତୁ *budget; }

  ସର୍ବସାଧାରଣ:
  hb_draw_funcs_t *funcs;
  ଶୂନ୍ୟ *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
