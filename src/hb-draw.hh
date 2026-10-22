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

#अगर_अघोषित HB_DRAW_HH
#घोषणा HB_DRAW_HH

#समावेश "hb.hh"


/*
 * hb_draw_funcs_t
 */

#घोषणा HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_to) \
  HB_DRAW_FUNC_IMPLEMENT (पंक्ति_to) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_to) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_to) \
  HB_DRAW_FUNC_IMPLEMENT (close_path) \
  HB_DRAW_FUNC_IMPLEMENT (set_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

काष्ठा hb_draw_funcs_t
{
  hb_object_header_t header;

  काष्ठा {
#घोषणा HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_DRAW_FUNC_IMPLEMENT
  } func;

  काष्ठा {
#घोषणा HB_DRAW_FUNC_IMPLEMENT(name) व्योम *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_DRAW_FUNC_IMPLEMENT
  } *user_तथ्य;

  काष्ठा {
#घोषणा HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  व्योम emit_move_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
		     भग्न to_x, भग्न to_y)
  { func.move_to (यह, draw_तथ्य, &st,
		  to_x, to_y,
		  !user_तथ्य ? nullptr : user_तथ्य->move_to); }
  व्योम emit_पंक्ति_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
		     भग्न to_x, भग्न to_y)
  { func.पंक्ति_to (यह, draw_तथ्य, &st,
		  to_x, to_y,
		  !user_तथ्य ? nullptr : user_तथ्य->पंक्ति_to); }
  व्योम emit_quadratic_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
			  भग्न control_x, भग्न control_y,
			  भग्न to_x, भग्न to_y)
  { func.quadratic_to (यह, draw_तथ्य, &st,
		       control_x, control_y,
		       to_x, to_y,
		       !user_तथ्य ? nullptr : user_तथ्य->quadratic_to); }
  व्योम emit_cubic_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
		      भग्न control1_x, भग्न control1_y,
		      भग्न control2_x, भग्न control2_y,
		      भग्न to_x, भग्न to_y)
  { func.cubic_to (यह, draw_तथ्य, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   to_x, to_y,
		   !user_तथ्य ? nullptr : user_तथ्य->cubic_to); }
  व्योम emit_close_path (व्योम *draw_तथ्य, hb_draw_state_t &st)
  { func.close_path (यह, draw_तथ्य, &st,
		     !user_तथ्य ? nullptr : user_तथ्य->close_path); }

  bool set_budget (व्योम *draw_तथ्य, int64_t budget)
  {
    अगर (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    वापस func.set_budget (यह, draw_तथ्य, budget,
			    !user_तथ्य ? nullptr : user_तथ्य->set_budget);
  }
  int64_t get_budget (व्योम *draw_तथ्य)
  { वापस func.get_budget (यह, draw_तथ्य,
			    !user_तथ्य ? nullptr : user_तथ्य->get_budget); }
  int64_t *get_budget_remaining_ptr (व्योम *draw_तथ्य)
  { वापस func.get_budget_remaining (यह, draw_तथ्य,
				      !user_तथ्य ? nullptr : user_तथ्य->get_budget_remaining); }


  व्योम
  HB_ALWAYS_INLINE
  move_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
	   भग्न to_x, भग्न to_y)
  {
    अगर (unlikely (st.path_open)) close_path (draw_तथ्य, st);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  व्योम
  HB_ALWAYS_INLINE
  पंक्ति_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
	   भग्न to_x, भग्न to_y)
  {
    अगर (unlikely (!st.path_open)) start_path (draw_तथ्य, st);

    emit_पंक्ति_to (draw_तथ्य, st, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  व्योम
  HB_ALWAYS_INLINE
  quadratic_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
		भग्न control_x, भग्न control_y,
		भग्न to_x, भग्न to_y)
  {
    अगर (unlikely (!st.path_open)) start_path (draw_तथ्य, st);

    emit_quadratic_to (draw_तथ्य, st, control_x, control_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  व्योम
  HB_ALWAYS_INLINE
  cubic_to (व्योम *draw_तथ्य, hb_draw_state_t &st,
	    भग्न control1_x, भग्न control1_y,
	    भग्न control2_x, भग्न control2_y,
	    भग्न to_x, भग्न to_y)
  {
    अगर (unlikely (!st.path_open)) start_path (draw_तथ्य, st);

    emit_cubic_to (draw_तथ्य, st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);

    st.current_x = to_x;
    st.current_y = to_y;
  }

  व्योम
  HB_ALWAYS_INLINE
  close_path (व्योम *draw_तथ्य, hb_draw_state_t &st)
  {
    अगर (likely (st.path_open))
    {
      अगर ((st.path_start_x != st.current_x) || (st.path_start_y != st.current_y))
	emit_पंक्ति_to (draw_तथ्य, st, st.path_start_x, st.path_start_y);
      emit_close_path (draw_तथ्य, st);
    }
    st.path_open = false;
    st.path_start_x = st.current_x = st.path_start_y = st.current_y = 0;
  }

  रक्षित:

  व्योम start_path (व्योम *draw_तथ्य, hb_draw_state_t &st)
  {
    निश्चित (!st.path_open);
    emit_move_to (draw_तथ्य, st, st.current_x, st.current_y);
    st.path_open = true;
    st.path_start_x = st.current_x;
    st.path_start_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

काष्ठा hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, व्योम *draw_तथ्य_2)
    : funcs {funcs_}, draw_तथ्य {draw_तथ्य_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_remaining_ptr (draw_तथ्य)}
  {
    अगर (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_path (); }

  HB_ALWAYS_INLINE
  व्योम move_to (भग्न to_x, भग्न to_y)
  {
    funcs->move_to (draw_तथ्य, st,
		    to_x, to_y);
  }
  HB_ALWAYS_INLINE
  व्योम पंक्ति_to (भग्न to_x, भग्न to_y)
  {
    funcs->पंक्ति_to (draw_तथ्य, st,
		    to_x, to_y);
  }
  व्योम
  HB_ALWAYS_INLINE
  quadratic_to (भग्न control_x, भग्न control_y,
		भग्न to_x, भग्न to_y)
  {
    funcs->quadratic_to (draw_तथ्य, st,
			 control_x, control_y,
			 to_x, to_y);
  }
  व्योम
  HB_ALWAYS_INLINE
  cubic_to (भग्न control1_x, भग्न control1_y,
	    भग्न control2_x, भग्न control2_y,
	    भग्न to_x, भग्न to_y)
  {
    funcs->cubic_to (draw_तथ्य, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     to_x, to_y);
  }
  HB_ALWAYS_INLINE
  व्योम close_path ()
  {
    funcs->close_path (draw_तथ्य, st);
  }

  int64_t &get_budget () { वापस *budget; }

  खुला:
  hb_draw_funcs_t *funcs;
  व्योम *draw_तथ्य;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_INTERNAL hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#पूर्ण_अगर /* HB_DRAW_HH */
