/*
 * Copyright © 2007  Chris Wilson
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 *
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_MUTEX_HH
#تعریف_کرو HB_MUTEX_HH

#شامل "hb.hh"


/* mutex */

/* We need external help for these */

#اگر defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_تالا) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_اختتام)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#وگرنہ_اگر !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#شامل <pthread.h>
قسمتعریف pthread_mutex_t hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)	pthread_mutex_init (M, خالی_اشاریہ)
#تعریف_کرو hb_mutex_impl_تالا(M)	pthread_mutex_lock (M)
#تعریف_کرو hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#تعریف_کرو hb_mutex_impl_اختتام(M)	pthread_mutex_destroy (M)


#وگرنہ_اگر !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

قسمتعریف CRITICAL_SECTION hb_mutex_impl_t;
#اگر !WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP) && WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_APP)
#تعریف_کرو hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#ورنہ
#تعریف_کرو hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#اگر_ختم
#تعریف_کرو hb_mutex_impl_تالا(M)	EnterCriticalSection (M)
#تعریف_کرو hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#تعریف_کرو hb_mutex_impl_اختتام(M)	DeleteCriticalSection (M)


#وگرنہ_اگر !defined(HB_NO_MT)

#شامل <mutex>
قسمتعریف std::mutex              hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)   HB_STMT_آغاز { نیا (M) hb_mutex_impl_t; } HB_STMT_انتہا
#تعریف_کرو hb_mutex_impl_تالا(M)   (M)->lock ()
#تعریف_کرو hb_mutex_impl_unlock(M) (M)->unlock ()
#تعریف_کرو hb_mutex_impl_اختتام(M) HB_STMT_آغاز { (M)->~hb_mutex_impl_t(); } HB_STMT_انتہا


#ورنہ /* defined(HB_NO_MT) */

قسمتعریف صحیح hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)	HB_STMT_آغاز {} HB_STMT_انتہا
#تعریف_کرو hb_mutex_impl_تالا(M)	HB_STMT_آغاز {} HB_STMT_انتہا
#تعریف_کرو hb_mutex_impl_unlock(M)	HB_STMT_آغاز {} HB_STMT_انتہا
#تعریف_کرو hb_mutex_impl_اختتام(M)	HB_STMT_آغاز {} HB_STMT_انتہا


#اگر_ختم


ساخت hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) حرف m[حجم (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (ثابت hb_mutex_t &) = حذف;
  hb_mutex_t &عامل= (ثابت hb_mutex_t &) = حذف;

#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  خالی init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  خالی lock   () { hb_mutex_impl_تالا   ((hb_mutex_impl_t *) m); }
  خالی unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  خالی fini   () { hb_mutex_impl_اختتام ((hb_mutex_impl_t *) m); }
#ہدایت GCC diagnostic نکالو
};

ساخت hb_تالا_t
{
  hb_تالا_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_تالا_t (hb_mutex_t *mutex_) : mutex (mutex_) { اگر (mutex) mutex->lock (); }
  ~hb_تالا_t () { اگر (mutex) mutex->unlock (); }

  hb_تالا_t (ثابت hb_تالا_t &) = حذف;
  hb_تالا_t &عامل= (ثابت hb_تالا_t &) = حذف;

  نجی:
  hb_mutex_t *mutex;
};


#اگر_ختم /* HB_MUTEX_HH */
