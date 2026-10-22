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

#অন্তর্ভাব "hb.hh"


/*
 * hb_draw_funcs_t
 */

#define HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (সারি_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

কাঠামো hb_draw_funcs_t
{
  hb_object_header_t header;

  কাঠামো {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } func;

  কাঠামো {
#define HB_DRAW_FUNC_IMPLEMENT(name) শূন্য *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *user_তথ্য;

  কাঠামো {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  শূন্য emit_move_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
		     দশমিক to_x, দশমিক to_y)
  { func.move_to (এই, draw_তথ্য, &st,
		  to_x, to_y,
		  !user_তথ্য ? nullptr : user_তথ্য->move_to); }
  শূন্য emit_সারি_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
		     দশমিক to_x, দশমিক to_y)
  { func.সারি_to (এই, draw_তথ্য, &st,
		  to_x, to_y,
		  !user_তথ্য ? nullptr : user_তথ্য->সারি_to); }
  শূন্য emit_quadratic_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
			  দশমিক control_x, দশমিক control_y,
			  দশমিক to_x, দশমিক to_y)
  { func.quadratic_to (এই, draw_তথ্য, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_তথ্য ? nullptr : user_তথ্য->quadratic_to); }
  শূন্য emit_cubic_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
		      দশমিক control1_x, দশমিক control1_y,
		      দশমিক control2_x, দশমিক control2_y,
		      দশমিক to_x, দশমিক to_y)
  { func.cubic_to (এই, draw_তথ্য, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_তথ্য ? nullptr : user_তথ্য->cubic_to); }
  শূন্য emit_close_path (শূন্য *draw_তথ্য, hb_draw_state_t &st)
  { func.close_path (এই, draw_তথ্য, &st,
		     !user_তথ্য ? nullptr : user_তথ্য->close_path); }

  যৌক্তিক set_budget (শূন্য *draw_তথ্য, int64_t budget)
  {
    যদি (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ফেরত func.set_budget (এই, draw_তথ্য, budget,
			    !user_তথ্য ? nullptr : user_তথ্য->set_budget);
  }
  int64_t get_budget (শূন্য *draw_তথ্য)
  { ফেরত func.get_budget (এই, draw_তথ্য,
			    !user_তথ্য ? nullptr : user_তথ্য->get_budget); }
  int64_t *get_budget_remaining_ptr (শূন্য *draw_তথ্য)
  { ফেরত func.get_budget_remaining (এই, draw_তথ্য,
				      !user_তথ্য ? nullptr : user_তথ্য->get_budget_remaining); }


  শূন্য
  HB_ALWAYS_INLINE
  move_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
	   দশমিক to_x, দশমিক to_y)
  {
    যদি (unlikely (st.path_open)) close_path (draw_তথ্য, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  সারি_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
	   দশমিক to_x, দশমিক to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_তথ্য, st);

    emit_সারি_to (draw_তথ্য, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  quadratic_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
		দশমিক control_x, দশমিক control_y,
		দশমিক to_x, দশমিক to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_তথ্য, st);

    emit_quadratic_to (draw_তথ্য, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  cubic_to (শূন্য *draw_তথ্য, hb_draw_state_t &st,
	    দশমিক control1_x, দশমিক control1_y,
	    দশমিক control2_x, দশমিক control2_y,
	    দশমিক to_x, দশমিক to_y)
  {
    যদি (unlikely (!st.path_open)) start_path (draw_তথ্য, st);

    emit_cubic_to (draw_তথ্য, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  শূন্য
  HB_ALWAYS_INLINE
  close_path (শূন্য *draw_তথ্য, hb_draw_state_t &st)
  {
    যদি (likely (st.path_open))
    {
      যদি ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_সারি_to (draw_তথ্য, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_তথ্য, st);
    }
    st.path_open = মিথ্যা;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  সুরক্ষিত:

  শূন্য start_path (শূন্য *draw_তথ্য, hb_draw_state_t &st)
  {
    প্রতিজ্ঞা (!st.path_open);
    emit_move_to (draw_তথ্য, st, st.current_x, st.current_y);
    st.path_open = সত্য;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

কাঠামো hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, শূন্য *draw_তথ্য_2)
    : funcs {funcs_}, draw_তথ্য {draw_তথ্য_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_তথ্য)}
  {
    যদি (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  শূন্য move_to (দশমিক to_x, দশমিক to_y)
  {
    funcs->move_to (draw_তথ্য, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  শূন্য সারি_to (দশমিক to_x, দশমিক to_y)
  {
    funcs->সারি_to (draw_তথ্য, st,
		    to_x, to_y);
  }
  শূন্য
  HB_ALWAYS_INLINE
  quadratic_to (দশমিক control_x, দশমিক control_y,
		দশমিক to_x, দশমিক to_y)
  {
    funcs->quadratic_to (draw_তথ্য, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  শূন্য
  HB_ALWAYS_INLINE
  cubic_to (দশমিক control1_x, দশমিক control1_y,
	    দশমিক control2_x, দশমিক control2_y,
	    দশমিক to_x, দশমিক to_y)
  {
    funcs->cubic_to (draw_তথ্য, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  শূন্য close_path ()
  {
    funcs->close_path (draw_তথ্য, st);
  }

  int64_t &get_budget () { ফেরত *budget; }

  সর্বজনীন:
  hb_draw_funcs_t *funcs;
  শূন্য *draw_তথ্য;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#endif /* HB_DRAW_HH */
