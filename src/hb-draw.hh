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

#அந்தர்பாவ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (வரி_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

கட்டமைப்பு hb_draw_funcs_t
{
  hb_object_header_t header;

  கட்டமைப்பு {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  கட்டமைப்பு {
#define HB_DRAW_FUNC_IMPLEMENT(name) வெறுமை *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  கட்டமைப்பு {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  வெறுமை emit_move_to (வெறுமை *draw_data, hb_draw_state_t &st,
		     தசம to_x, தசம to_y)
  { func.move_to (இது, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  வெறுமை emit_வரி_to (வெறுமை *draw_data, hb_draw_state_t &st,
		     தசம to_x, தசம to_y)
  { func.வரி_to (இது, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->வரி_to); }
  வெறுமை emit_quadratic_to (வெறுமை *draw_data, hb_draw_state_t &st,
			  தசம control_x, தசம control_y,
			  தசம to_x, தசம to_y)
  { func.quadratic_to (இது, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  வெறுமை emit_cubic_to (வெறுமை *draw_data, hb_draw_state_t &st,
		      தசம control1_x, தசம control1_y,
		      தசம control2_x, தசம control2_y,
		      தசம to_x, தசம to_y)
  { func.cubic_to (இது, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  வெறுமை emit_close_path (வெறுமை *draw_data, hb_draw_state_t &st)
  { func.close_path (இது, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  தருக்க set_budget (வெறுமை *draw_data, int64_t budget)
  {
    எனில் (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    திரும்பு func.set_budget (இது, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (வெறுமை *draw_data)
  { திரும்பு func.get_budget (இது, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (வெறுமை *draw_data)
  { திரும்பு func.get_budget_remaining (இது, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  வெறுமை
  HB_ALWAYS_INLINE
  move_to (வெறுமை *draw_data, hb_draw_state_t &st,
	   தசம to_x, தசம to_y)
  {
    எனில் (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  வெறுமை
  HB_ALWAYS_INLINE
  வரி_to (வெறுமை *draw_data, hb_draw_state_t &st,
	   தசம to_x, தசம to_y)
  {
    எனில் (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_வரி_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  வெறுமை
  HB_ALWAYS_INLINE
  quadratic_to (வெறுமை *draw_data, hb_draw_state_t &st,
		தசம control_x, தசம control_y,
		தசம to_x, தசம to_y)
  {
    எனில் (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  வெறுமை
  HB_ALWAYS_INLINE
  cubic_to (வெறுமை *draw_data, hb_draw_state_t &st,
	    தசம control1_x, தசம control1_y,
	    தசம control2_x, தசம control2_y,
	    தசம to_x, தசம to_y)
  {
    எனில் (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  வெறுமை
  HB_ALWAYS_INLINE
  close_path (வெறுமை *draw_data, hb_draw_state_t &st)
  {
    எனில் (likely (st.path_open))
    {
      எனில் ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_வரி_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = பொய்;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  பாதுகாக்கப்பட்ட:

  வெறுமை start_path (வெறுமை *draw_data, hb_draw_state_t &st)
  {
    ப்ரதிஜ்ஞா (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = உண்மை;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

கட்டமைப்பு hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, வெறுமை *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    எனில் (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  வெறுமை move_to (தசம to_x, தசம to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  வெறுமை வரி_to (தசம to_x, தசம to_y)
  {
    funcs->வரி_to (draw_data, st,
		    to_x, to_y);
  }
  வெறுமை
  HB_ALWAYS_INLINE
  quadratic_to (தசம control_x, தசம control_y,
		தசம to_x, தசம to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  வெறுமை
  HB_ALWAYS_INLINE
  cubic_to (தசம control1_x, தசம control1_y,
	    தசம control2_x, தசம control2_y,
	    தசம to_x, தசம to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  வெறுமை close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { திரும்பு *budget; }

  பொது:
  hb_draw_funcs_t *funcs;
  வெறுமை *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
