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

#অন্তর্ভাৱ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (সাৰি_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

সংরচনা hb_draw_funcs_t
{
  hb_object_header_t header;

  সংরচনা {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  সংরচনা {
#define HB_DRAW_FUNC_IMPLEMENT(name) শূন্য *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_data;

  সংরচনা {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  শূন্য emit_move_to (শূন্য *draw_data, hb_draw_state_t &st,
		     ভিন্ন to_x, ভিন্ন to_y)
  { func.move_to (এতত্, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->move_to); }
  শূন্য emit_সাৰি_to (শূন্য *draw_data, hb_draw_state_t &st,
		     ভিন্ন to_x, ভিন্ন to_y)
  { func.সাৰি_to (এতত্, draw_data, &st,
		  to_x, to_y,
		  !user_data ? nullptr : user_data->সাৰি_to); }
  শূন্য emit_quadratic_to (শূন্য *draw_data, hb_draw_state_t &st,
			  ভিন্ন control_x, ভিন্ন control_y,
			  ভিন্ন to_x, ভিন্ন to_y)
  { func.quadratic_to (এতত্, draw_data, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_data ? nullptr : user_data->quadratic_to); }
  শূন্য emit_cubic_to (শূন্য *draw_data, hb_draw_state_t &st,
		      ভিন্ন control1_x, ভিন্ন control1_y,
		      ভিন্ন control2_x, ভিন্ন control2_y,
		      ভিন্ন to_x, ভিন্ন to_y)
  { func.cubic_to (এতত্, draw_data, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_data ? nullptr : user_data->cubic_to); }
  শূন্য emit_close_path (শূন্য *draw_data, hb_draw_state_t &st)
  { func.close_path (এতত্, draw_data, &st,
		     !user_data ? nullptr : user_data->close_path); }

  তার্কিক set_budget (শূন্য *draw_data, int64_t budget)
  {
    যদি (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ঘূৰাই func.set_budget (এতত্, draw_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (শূন্য *draw_data)
  { ঘূৰাই func.get_budget (এতত্, draw_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (শূন্য *draw_data)
  { ঘূৰাই func.get_budget_remaining (এতত্, draw_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  শূন্য
  HB_ALWAYS_INLINE
  move_to (শূন্য *draw_data, hb_draw_state_t &st,
	   ভিন্ন to_x, ভিন্ন to_y)
  {
    যদি (unlikely (st.path_open)) close_path (draw_data, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  সাৰি_to (শূন্য *draw_data, hb_draw_state_t &st,
	   ভিন্ন to_x, ভিন্ন to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_সাৰি_to (draw_data, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  quadratic_to (শূন্য *draw_data, hb_draw_state_t &st,
		ভিন্ন control_x, ভিন্ন control_y,
		ভিন্ন to_x, ভিন্ন to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_quadratic_to (draw_data, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  cubic_to (শূন্য *draw_data, hb_draw_state_t &st,
	    ভিন্ন control1_x, ভিন্ন control1_y,
	    ভিন্ন control2_x, ভিন্ন control2_y,
	    ভিন্ন to_x, ভিন্ন to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_data, st);

    emit_cubic_to (draw_data, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  close_path (শূন্য *draw_data, hb_draw_state_t &st)
  {
    যদি (likely (st.path_open))
    {
      যদি ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_সাৰি_to (draw_data, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_data, st);
    }
    st.path_open = মিছা;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  রক্ষিত:

  শূন্য start_path (শূন্য *draw_data, hb_draw_state_t &st)
  {
    প্রতিজ্ঞা (!st.path_open);
    emit_move_to (draw_data, st, st.current_x, st.current_y);
    st.path_open = সঁচা;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

সংরচনা hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, শূন্য *draw_data_)
    : funcs {funcs_}, draw_data {draw_data_}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_data)}
  {
    যদি (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  শূন্য move_to (ভিন্ন to_x, ভিন্ন to_y)
  {
    funcs->move_to (draw_data, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  শূন্য সাৰি_to (ভিন্ন to_x, ভিন্ন to_y)
  {
    funcs->সাৰি_to (draw_data, st,
		    to_x, to_y);
  }
  শূন্য
  HB_ALWAYS_INLINE
  quadratic_to (ভিন্ন control_x, ভিন্ন control_y,
		ভিন্ন to_x, ভিন্ন to_y)
  {
    funcs->quadratic_to (draw_data, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  শূন্য
  HB_ALWAYS_INLINE
  cubic_to (ভিন্ন control1_x, ভিন্ন control1_y,
	    ভিন্ন control2_x, ভিন্ন control2_y,
	    ভিন্ন to_x, ভিন্ন to_y)
  {
    funcs->cubic_to (draw_data, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  শূন্য close_path ()
  {
    funcs->close_path (draw_data, st);
  }

  int64_t &get_budget () { ঘূৰাই *budget; }

  সার্ৱজনিক:
  hb_draw_funcs_t *funcs;
  শূন্য *draw_data;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
