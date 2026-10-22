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

#నిర్వచితం_కాకపోతే HB_DRAW_HH
#నిర్వచించు HB_DRAW_HH

#అన్తర్భావ "hb.hh"


/*
 * hb_draw_funcs_t
 */

#నిర్వచించు HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS \
  HB_DRAW_FUNC_IMPLEMENT (move_గమ్యం) \
  HB_DRAW_FUNC_IMPLEMENT (పంక్తి_గమ్యం) \
  HB_DRAW_FUNC_IMPLEMENT (quadratic_గమ్యం) \
  HB_DRAW_FUNC_IMPLEMENT (cubic_గమ్యం) \
  HB_DRAW_FUNC_IMPLEMENT (close_మార్గం) \
  HB_DRAW_FUNC_IMPLEMENT (సమితి_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget) \
  HB_DRAW_FUNC_IMPLEMENT (get_budget_మిగిలిన) \
  /* ^--- Add new callbacks here */

నిర్మాణం hb_draw_funcs_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  నిర్మాణం {
#నిర్వచించు HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_DRAW_FUNC_IMPLEMENT
  } func;

  నిర్మాణం {
#నిర్వచించు HB_DRAW_FUNC_IMPLEMENT(name) శూన్య *name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_DRAW_FUNC_IMPLEMENT
  } *user_దత్తాంశం;

  నిర్మాణం {
#నిర్వచించు HB_DRAW_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_DRAW_FUNC_IMPLEMENT
  } *destroy;

  శూన్య emit_move_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
		     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  { func.move_గమ్యం (ఇది, draw_దత్తాంశం, &st,
		  గమ్యం_x, గమ్యం_y,
		  !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->move_గమ్యం); }
  శూన్య emit_పంక్తి_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
		     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  { func.పంక్తి_గమ్యం (ఇది, draw_దత్తాంశం, &st,
		  గమ్యం_x, గమ్యం_y,
		  !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->పంక్తి_గమ్యం); }
  శూన్య emit_quadratic_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
			  దశాంశం control_x, దశాంశం control_y,
			  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  { func.quadratic_గమ్యం (ఇది, draw_దత్తాంశం, &st,
		       control_x, control_y,
		       గమ్యం_x, గమ్యం_y,
		       !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->quadratic_గమ్యం); }
  శూన్య emit_cubic_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
		      దశాంశం control1_x, దశాంశం control1_y,
		      దశాంశం control2_x, దశాంశం control2_y,
		      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  { func.cubic_గమ్యం (ఇది, draw_దత్తాంశం, &st,
		   control1_x, control1_y,
		   control2_x, control2_y,
		   గమ్యం_x, గమ్యం_y,
		   !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->cubic_గమ్యం); }
  శూన్య emit_close_మార్గం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st)
  { func.close_మార్గం (ఇది, draw_దత్తాంశం, &st,
		     !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->close_మార్గం); }

  తార్కిక సమితి_budget (శూన్య *draw_దత్తాంశం, int64_t budget)
  {
    ఒకవేళ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    తిరిగి func.సమితి_budget (ఇది, draw_దత్తాంశం, budget,
			    !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->సమితి_budget);
  }
  int64_t get_budget (శూన్య *draw_దత్తాంశం)
  { తిరిగి func.get_budget (ఇది, draw_దత్తాంశం,
			    !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->get_budget); }
  int64_t *get_budget_మిగిలిన_ptr (శూన్య *draw_దత్తాంశం)
  { తిరిగి func.get_budget_మిగిలిన (ఇది, draw_దత్తాంశం,
				      !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->get_budget_మిగిలిన); }


  శూన్య
  HB_ALWAYS_INLINE
  move_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
	   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    ఒకవేళ (unlikely (st.మార్గం_open)) close_మార్గం (draw_దత్తాంశం, st);

    st.current_x = గమ్యం_x;
    st.current_y = గమ్యం_y;
  }

  శూన్య
  HB_ALWAYS_INLINE
  పంక్తి_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
	   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    ఒకవేళ (unlikely (!st.మార్గం_open)) ప్రారంభం_మార్గం (draw_దత్తాంశం, st);

    emit_పంక్తి_గమ్యం (draw_దత్తాంశం, st, గమ్యం_x, గమ్యం_y);

    st.current_x = గమ్యం_x;
    st.current_y = గమ్యం_y;
  }

  శూన్య
  HB_ALWAYS_INLINE
  quadratic_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
		దశాంశం control_x, దశాంశం control_y,
		దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    ఒకవేళ (unlikely (!st.మార్గం_open)) ప్రారంభం_మార్గం (draw_దత్తాంశం, st);

    emit_quadratic_గమ్యం (draw_దత్తాంశం, st, control_x, control_y, గమ్యం_x, గమ్యం_y);

    st.current_x = గమ్యం_x;
    st.current_y = గమ్యం_y;
  }

  శూన్య
  HB_ALWAYS_INLINE
  cubic_గమ్యం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st,
	    దశాంశం control1_x, దశాంశం control1_y,
	    దశాంశం control2_x, దశాంశం control2_y,
	    దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    ఒకవేళ (unlikely (!st.మార్గం_open)) ప్రారంభం_మార్గం (draw_దత్తాంశం, st);

    emit_cubic_గమ్యం (draw_దత్తాంశం, st, control1_x, control1_y, control2_x, control2_y, గమ్యం_x, గమ్యం_y);

    st.current_x = గమ్యం_x;
    st.current_y = గమ్యం_y;
  }

  శూన్య
  HB_ALWAYS_INLINE
  close_మార్గం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st)
  {
    ఒకవేళ (likely (st.మార్గం_open))
    {
      ఒకవేళ ((st.మార్గం_ప్రారంభం_x != st.current_x) || (st.మార్గం_ప్రారంభం_y != st.current_y))
	emit_పంక్తి_గమ్యం (draw_దత్తాంశం, st, st.మార్గం_ప్రారంభం_x, st.మార్గం_ప్రారంభం_y);
      emit_close_మార్గం (draw_దత్తాంశం, st);
    }
    st.మార్గం_open = అబద్ధం;
    st.మార్గం_ప్రారంభం_x = st.current_x = st.మార్గం_ప్రారంభం_y = st.current_y = 0;
  }

  రక్షిత:

  శూన్య ప్రారంభం_మార్గం (శూన్య *draw_దత్తాంశం, hb_draw_state_t &st)
  {
    ప్రతిజ్ఞా (!st.మార్గం_open);
    emit_move_గమ్యం (draw_దత్తాంశం, st, st.current_x, st.current_y);
    st.మార్గం_open = నిజం;
    st.మార్గం_ప్రారంభం_x = st.current_x;
    st.మార్గం_ప్రారంభం_y = st.current_y;
  }
};
DECLARE_NULL_INSTANCE (hb_draw_funcs_t);

నిర్మాణం hb_draw_session_t
{
  hb_draw_session_t (hb_draw_funcs_t *funcs_, శూన్య *draw_దత్తాంశం_2)
    : funcs {funcs_}, draw_దత్తాంశం {draw_దత్తాంశం_2}, st HB_DRAW_STATE_DEFAULT,
      local_budget {HB_BUDGET_GLYPH}, budget {funcs->get_budget_మిగిలిన_ptr (draw_దత్తాంశం)}
  {
    ఒకవేళ (!budget)
      budget = &local_budget;
  }

  ~hb_draw_session_t () { close_మార్గం (); }

  HB_ALWAYS_INLINE
  శూన్య move_గమ్యం (దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    funcs->move_గమ్యం (draw_దత్తాంశం, st,
		    గమ్యం_x, గమ్యం_y);
  }
  HB_ALWAYS_INLINE
  శూన్య పంక్తి_గమ్యం (దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    funcs->పంక్తి_గమ్యం (draw_దత్తాంశం, st,
		    గమ్యం_x, గమ్యం_y);
  }
  శూన్య
  HB_ALWAYS_INLINE
  quadratic_గమ్యం (దశాంశం control_x, దశాంశం control_y,
		దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    funcs->quadratic_గమ్యం (draw_దత్తాంశం, st,
			 control_x, control_y,
			 గమ్యం_x, గమ్యం_y);
  }
  శూన్య
  HB_ALWAYS_INLINE
  cubic_గమ్యం (దశాంశం control1_x, దశాంశం control1_y,
	    దశాంశం control2_x, దశాంశం control2_y,
	    దశాంశం గమ్యం_x, దశాంశం గమ్యం_y)
  {
    funcs->cubic_గమ్యం (draw_దత్తాంశం, st,
		     control1_x, control1_y,
		     control2_x, control2_y,
		     గమ్యం_x, గమ్యం_y);
  }
  HB_ALWAYS_INLINE
  శూన్య close_మార్గం ()
  {
    funcs->close_మార్గం (draw_దత్తాంశం, st);
  }

  int64_t &get_budget () { తిరిగి *budget; }

  బహిరంగ:
  hb_draw_funcs_t *funcs;
  శూన్య *draw_దత్తాంశం;
  hb_draw_state_t st;
  int64_t local_budget;
  int64_t *budget;
};


HB_అంతర్గత hb_draw_funcs_t *
hb_draw_extents_get_funcs ();


#అయితే_ముగింపు /* HB_DRAW_HH */
