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

#अन्तर्भाव "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (ओळ_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

संरचना hb_draw_funcs_t
{
  hb_object_header_t header;

  संरचना {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  संरचना {
#define HB_DRAW_FUNC_IMPLEMENT(name) शून्य *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  संरचना {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  शून्य emit_move_to (शून्य *draw_data, hb_draw_state_t &st,
		     दशांश to_x, दशांश to_y)
  { func.move_to (हे, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  शून्य emit_ओळ_to (शून्य *draw_data, hb_draw_state_t &st,
		     दशांश to_x, दशांश to_y)
  { func.ओळ_to (हे, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->ओळ_to); }
  शून्य emit_quadratic_to (शून्य *draw_data, hb_draw_state_t &st,
			  दशांश control_x, दशांश control_y,
			  दशांश to_x, दशांश to_y)
  { func.quadratic_to (हे, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  शून्य emit_cubic_to (शून्य *draw_data, hb_draw_state_t &st,
		      दशांश control1_x, दशांश control1_y,
		      दशांश control2_x, दशांश control2_y,
		      दशांश to_x, दशांश to_y)
  { func.cubic_to (हे, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  शून्य emit_close_path (शून्य *draw_data, hb_draw_state_t &st)
  { func.close_path (हे, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  तार्किक set_budget (शून्य *draw_data, int64_t budget)
  {
    जर (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    परत func.set_budget (हे, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (शून्य *draw_data)
  { परत func.get_budget (हे, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (शून्य *draw_data)
  { परत func.get_budget_remaining (हे, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  शून्य
  HB_ALWAYS_INLINE
  move_to (शून्य *draw_data, hb_draw_state_t &st,
	   दशांश to_x, दशांश to_y)
  {
    जर (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  शून्य
  HB_ALWAYS_INLINE
  ओळ_to (शून्य *draw_data, hb_draw_state_t &st,
	   दशांश to_x, दशांश to_y)
  {
    जर (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_ओळ_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  शून्य
  HB_ALWAYS_INLINE
  quadratic_to (शून्य *draw_data, hb_draw_state_t &st,
		दशांश control_x, दशांश control_y,
		दशांश to_x, दशांश to_y)
  {
    जर (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  शून्य
  HB_ALWAYS_INLINE
  cubic_to (शून्य *draw_data, hb_draw_state_t &st,
	    दशांश control1_x, दशांश control1_y,
	    दशांश control2_x, दशांश control2_y,
	    दशांश to_x, दशांश to_y)
  {
    जर (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  शून्य
  HB_ALWAYS_INLINE
  close_path (शून्य *draw_data, hb_draw_state_t &st)
  {
    जर (likely (st.path_open))
    {
      जर ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_ओळ_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = असत्य;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  संरक्षित:

  शून्य start_path (शून्य *draw_data, hb_draw_state_t &st)
  {
    प्रतिज्ञा (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = सत्य;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

संरचना hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, शून्य *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    जर (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  शून्य move_to (दशांश to_x, दशांश to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  शून्य ओळ_to (दशांश to_x, दशांश to_y)
  {
    funcs->ओळ_to (draw_data, st,
		    to_x, to_y);
  }
  शून्य
  HB_ALWAYS_INLINE
  quadratic_to (दशांश control_x, दशांश control_y,
		दशांश to_x, दशांश to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  शून्य
  HB_ALWAYS_INLINE
  cubic_to (दशांश control1_x, दशांश control1_y,
	    दशांश control2_x, दशांश control2_y,
	    दशांश to_x, दशांश to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  शून्य close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { परत *budget; }

  सार्वजनिक:
  hb_draw_funcs_t *funcs;
  शून्य *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
