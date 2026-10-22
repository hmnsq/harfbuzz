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

#جے_بنا_تعریف HB_MUTEX_HH
#تعریف_کرو HB_MUTEX_HH

#ضمن "hb.hh"


/* mutex */

/* We need external help for these */

#إذا defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_جندرا) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_انت)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#وإلاإذا !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#ضمن <pthread.h>
تعريفنوع pthread_mutex_t hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)	pthread_mutex_init (M, خالی_سوچک)
#تعریف_کرو hb_mutex_impl_جندرا(M)	pthread_mutex_lock (M)
#تعریف_کرو hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#تعریف_کرو hb_mutex_impl_انت(M)	pthread_mutex_destroy (M)


#وإلاإذا !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

تعريفنوع CRITICAL_SECTION hb_mutex_impl_t;
#إذا !WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP) && WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_APP)
#تعریف_کرو hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#وإلا
#تعریف_کرو hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#جے_انت
#تعریف_کرو hb_mutex_impl_جندرا(M)	EnterCriticalSection (M)
#تعریف_کرو hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#تعریف_کرو hb_mutex_impl_انت(M)	DeleteCriticalSection (M)


#وإلاإذا !defined(HB_NO_MT)

#ضمن <mutex>
تعريفنوع std::mutex              hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)   HB_STMT_شروع { جديد (M) hb_mutex_impl_t; } HB_STMT_سرا
#تعریف_کرو hb_mutex_impl_جندرا(M)   (M)->lock ()
#تعریف_کرو hb_mutex_impl_unlock(M) (M)->unlock ()
#تعریف_کرو hb_mutex_impl_انت(M) HB_STMT_شروع { (M)->~hb_mutex_impl_t(); } HB_STMT_سرا


#وإلا /* defined(HB_NO_MT) */

تعريفنوع صحيح hb_mutex_impl_t;
#تعریف_کرو hb_mutex_impl_init(M)	HB_STMT_شروع {} HB_STMT_سرا
#تعریف_کرو hb_mutex_impl_جندرا(M)	HB_STMT_شروع {} HB_STMT_سرا
#تعریف_کرو hb_mutex_impl_unlock(M)	HB_STMT_شروع {} HB_STMT_سرا
#تعریف_کرو hb_mutex_impl_انت(M)	HB_STMT_شروع {} HB_STMT_سرا


#جے_انت


بنية hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) حرف m[حجم (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (ثابت hb_mutex_t &) = مٹاؤ;
  hb_mutex_t &عامل= (ثابت hb_mutex_t &) = مٹاؤ;

#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  فراغ init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  فراغ lock   () { hb_mutex_impl_جندرا   ((hb_mutex_impl_t *) m); }
  فراغ unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  فراغ fini   () { hb_mutex_impl_انت ((hb_mutex_impl_t *) m); }
#ہدایت GCC diagnostic نکالو
};

بنية hb_جندرا_t
{
  hb_جندرا_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_جندرا_t (hb_mutex_t *mutex_) : mutex (mutex_) { إذا (mutex) mutex->lock (); }
  ~hb_جندرا_t () { إذا (mutex) mutex->unlock (); }

  hb_جندرا_t (ثابت hb_جندرا_t &) = مٹاؤ;
  hb_جندرا_t &عامل= (ثابت hb_جندرا_t &) = مٹاؤ;

  خاص:
  hb_mutex_t *mutex;
};


#جے_انت /* HB_MUTEX_HH */
