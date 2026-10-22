/*
 * Copyright © 2022 Behdad Esfahbod
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

#నిర్వచితం_కాకపోతే HB_CPLUSPLUS_HH
#నిర్వచించు HB_CPLUSPLUS_HH

#అన్తర్భావ "hb.h"

#నిర్వచితమైతే __cplusplus

#అన్తర్భావ <functional>
#అన్తర్భావ <utility>

#ఒకవేళ 0
#ఒకవేళ !(__cplusplus >= 201103L)
#దోషం "HarfBuzz C++ helpers require C++11"
#అయితే_ముగింపు
#అయితే_ముగింపు

నామాకాశ hb {


మూస <రకం_పేరు T>
నిర్మాణం vtable;

మూస <రకం_పేరు T>
నిర్మాణం shared_ptr
{
  ఉపయోగించి element_type = T;

  ఉపయోగించి v = vtable<T>;

  స్పష్టమైన shared_ptr (T *p = శూన్యసూచిక) : p (p) {}
  shared_ptr (స్థిరరాశి shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = శూన్యసూచిక; }
  shared_ptr& క్రియాకారకం = (స్థిరరాశి shared_ptr &o) { ఒకవేళ (p != o.p) { destroy (); p = o.p; reference (); } తిరిగి *ఇది; }
  shared_ptr& క్రియాకారకం = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = శూన్యసూచిక; తిరిగి *ఇది; }
  ~shared_ptr () { v::destroy (p); p = శూన్యసూచిక; }

  T* get() స్థిరరాశి { తిరిగి p; }

  శూన్య swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  మిత్ర శూన్య swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  క్రియాకారకం T * () స్థిరరాశి { తిరిగి p; }
  T& క్రియాకారకం * () స్థిరరాశి { తిరిగి *get (); }
  T* క్రియాకారకం -> () స్థిరరాశి { తిరిగి get (); }
  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి p; }
  తార్కిక క్రియాకారకం == (స్థిరరాశి shared_ptr &o) స్థిరరాశి { తిరిగి p == o.p; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి shared_ptr &o) స్థిరరాశి { తిరిగి p != o.p; }

  స్థిర T* get_ఖాళీలు() { తిరిగి v::get_ఖాళీలు (); }
  T* reference() { తిరిగి v::reference (p); }
  శూన్య destroy() { v::destroy (p); }
  శూన్య సమితి_user_దత్తాంశం (hb_user_దత్తాంశం_కీలకం_t *key,
		      శూన్య *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::సమితి_user_దత్తాంశం (p, key, value, destroy, replace); } 
  శూన్య * get_user_దత్తాంశం (hb_user_దత్తాంశం_కీలకం_t *key) { తిరిగి v::get_user_దత్తాంశం (p, key); }

  అంతరంగ:
  T *p;
};

మూస<రకం_పేరు T> నిర్మాణం is_shared_ptr : std::false_type {};
మూస<రకం_పేరు T> నిర్మాణం is_shared_ptr<shared_ptr<T>> : std::true_type {};

మూస <రకం_పేరు T>
నిర్మాణం unique_ptr
{
  ఉపయోగించి element_type = T;

  ఉపయోగించి v = vtable<T>;

  స్పష్టమైన unique_ptr (T *p = శూన్యసూచిక) : p (p) {}
  unique_ptr (స్థిరరాశి unique_ptr &o) = తొలగించు;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = శూన్యసూచిక; }
  unique_ptr& క్రియాకారకం = (స్థిరరాశి unique_ptr &o) = తొలగించు;
  unique_ptr& క్రియాకారకం = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = శూన్యసూచిక; తిరిగి *ఇది; }
  ~unique_ptr () { v::destroy (p); p = శూన్యసూచిక; }

  T* get() స్థిరరాశి { తిరిగి p; }
  T* release () { T* v = p; p = శూన్యసూచిక; తిరిగి v; }

  శూన్య swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  మిత్ర శూన్య swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  క్రియాకారకం T * () స్థిరరాశి { తిరిగి p; }
  T& క్రియాకారకం * () స్థిరరాశి { తిరిగి *get (); }
  T* క్రియాకారకం -> () స్థిరరాశి { తిరిగి get (); }
  క్రియాకారకం తార్కిక () { తిరిగి p; }

  అంతరంగ:
  T *p;
};

మూస<రకం_పేరు T> నిర్మాణం is_unique_ptr : std::false_type {};
మూస<రకం_పేరు T> నిర్మాణం is_unique_ptr<unique_ptr<T>> : std::true_type {};

మూస <రకం_పేరు T,
	  T * (*get_ఖాళీలు_2) (శూన్య),
	  T * (*_reference) (T *),
	  శూన్య (*_destroy) (T *),
	  hb_bool_t (*సమితి_user_దత్తాంశం_2) (T *,
				       hb_user_దత్తాంశం_కీలకం_t *,
				       శూన్య *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  శూన్య * (*get_user_దత్తాంశం_2) (స్థిరరాశి T *,
				    hb_user_దత్తాంశం_కీలకం_t *)>
నిర్మాణం vtable_t
{
  స్థిర constexpr స్వయంచాలక get_ఖాళీలు = get_ఖాళీలు_2;
  స్థిర constexpr స్వయంచాలక reference = _reference;
  స్థిర constexpr స్వయంచాలక destroy = _destroy;
  స్థిర constexpr స్వయంచాలక సమితి_user_దత్తాంశం = సమితి_user_దత్తాంశం_2;
  స్థిర constexpr స్వయంచాలక get_user_దత్తాంశం = get_user_దత్తాంశం_2;
};

#నిర్వచించు HB_DEFINE_VTABLE(name, ఖాళీలు_2) \
	మూస<> \
	నిర్మాణం vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			ఖాళీలు_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (బఫరు, &hb_బఫరు_get_ఖాళీలు);
HB_DEFINE_VTABLE (blob, &hb_blob_get_ఖాళీలు);
HB_DEFINE_VTABLE (face, &hb_face_get_ఖాళీలు);
HB_DEFINE_VTABLE (font, &hb_font_get_ఖాళీలు);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_ఖాళీలు);
HB_DEFINE_VTABLE (map, &hb_map_get_ఖాళీలు);
HB_DEFINE_VTABLE (సమితి, &hb_సమితి_get_ఖాళీలు);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_ఖాళీలు);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_ఖాళీలు);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_ఖాళీలు);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_ఖాళీలు);


#నిర్వచితమైతే HB_SUBSET_H

HB_DEFINE_VTABLE (subset_ఆదానం, శూన్యసూచిక);
HB_DEFINE_VTABLE (subset_plan, శూన్యసూచిక);

#అయితే_ముగింపు


#నిర్వచితమైతే HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, శూన్యసూచిక);
HB_DEFINE_VTABLE (raster_draw, శూన్యసూచిక);
HB_DEFINE_VTABLE (raster_paint, శూన్యసూచిక);

#అయితే_ముగింపు


#నిర్వచితమైతే HB_సదిశ_H

HB_DEFINE_VTABLE (సదిశ_draw, శూన్యసూచిక);
HB_DEFINE_VTABLE (సదిశ_paint, శూన్యసూచిక);

#అయితే_ముగింపు


#నిర్వచితమైతే HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, శూన్యసూచిక);

#అయితే_ముగింపు

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
నామాకాశ std {


మూస<రకం_పేరు T>
నిర్మాణం hash<hb::shared_ptr<T>>
{
    std::size_t క్రియాకారకం()(స్థిరరాశి hb::shared_ptr<T>& v) స్థిరరాశి noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        తిరిగి h;
    }
};

మూస<రకం_పేరు T>
నిర్మాణం hash<hb::unique_ptr<T>>
{
    std::size_t క్రియాకారకం()(స్థిరరాశి hb::unique_ptr<T>& v) స్థిరరాశి noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        తిరిగి h;
    }
};


} // namespace std

#అయితే_ముగింపు /* __cplusplus */

#అయితే_ముగింపు /* HB_CPLUSPLUS_HH */
