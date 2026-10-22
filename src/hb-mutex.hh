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

#جيڪڏهن_وصف_نه_ٿيل HB_MUTEX_HH
#وصف_ڪريو HB_MUTEX_HH

#ضمن "hb.hh"


/* mutex */

/* We need external help for these */

#إذا defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_تالو) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_پڄاڻي)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#وإلاإذا !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#ضمن <pthread.h>
تعريفنوع pthread_mutex_t hb_mutex_impl_t;
#وصف_ڪريو hb_mutex_impl_init(M)	pthread_mutex_init (M, خالي_اشارو)
#وصف_ڪريو hb_mutex_impl_تالو(M)	pthread_mutex_lock (M)
#وصف_ڪريو hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#وصف_ڪريو hb_mutex_impl_پڄاڻي(M)	pthread_mutex_destroy (M)


#وإلاإذا !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

تعريفنوع CRITICAL_SECTION hb_mutex_impl_t;
#إذا !WINAPI_FAMILY_حصا_ڪريو(WINAPI_حصا_ڪريو_DESKTOP) && WINAPI_FAMILY_حصا_ڪريو(WINAPI_حصا_ڪريو_APP)
#وصف_ڪريو hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#وإلا
#وصف_ڪريو hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#جيڪڏهن_پورو
#وصف_ڪريو hb_mutex_impl_تالو(M)	EnterCriticalSection (M)
#وصف_ڪريو hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#وصف_ڪريو hb_mutex_impl_پڄاڻي(M)	DeleteCriticalSection (M)


#وإلاإذا !defined(HB_NO_MT)

#ضمن <mutex>
تعريفنوع std::mutex              hb_mutex_impl_t;
#وصف_ڪريو hb_mutex_impl_init(M)   HB_STMT_شروعات { جديد (M) hb_mutex_impl_t; } HB_STMT_پڇاڙي
#وصف_ڪريو hb_mutex_impl_تالو(M)   (M)->lock ()
#وصف_ڪريو hb_mutex_impl_unlock(M) (M)->unlock ()
#وصف_ڪريو hb_mutex_impl_پڄاڻي(M) HB_STMT_شروعات { (M)->~hb_mutex_impl_t(); } HB_STMT_پڇاڙي


#وإلا /* defined(HB_NO_MT) */

تعريفنوع صحيح hb_mutex_impl_t;
#وصف_ڪريو hb_mutex_impl_init(M)	HB_STMT_شروعات {} HB_STMT_پڇاڙي
#وصف_ڪريو hb_mutex_impl_تالو(M)	HB_STMT_شروعات {} HB_STMT_پڇاڙي
#وصف_ڪريو hb_mutex_impl_unlock(M)	HB_STMT_شروعات {} HB_STMT_پڇاڙي
#وصف_ڪريو hb_mutex_impl_پڄاڻي(M)	HB_STMT_شروعات {} HB_STMT_پڇاڙي


#جيڪڏهن_پورو


بنية hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) حرف m[حجم (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (ثابت hb_mutex_t &) = ميٽيو;
  hb_mutex_t &عامل= (ثابت hb_mutex_t &) = ميٽيو;

#هدايت GCC diagnostic push
#هدايت GCC diagnostic ignored "-Wcast-align"
  فراغ init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  فراغ lock   () { hb_mutex_impl_تالو   ((hb_mutex_impl_t *) m); }
  فراغ unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  فراغ fini   () { hb_mutex_impl_پڄاڻي ((hb_mutex_impl_t *) m); }
#هدايت GCC diagnostic ڪڍو
};

بنية hb_تالو_t
{
  hb_تالو_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_تالو_t (hb_mutex_t *mutex_) : mutex (mutex_) { إذا (mutex) mutex->lock (); }
  ~hb_تالو_t () { إذا (mutex) mutex->unlock (); }

  hb_تالو_t (ثابت hb_تالو_t &) = ميٽيو;
  hb_تالو_t &عامل= (ثابت hb_تالو_t &) = ميٽيو;

  خاص:
  hb_mutex_t *mutex;
};


#جيڪڏهن_پورو /* HB_MUTEX_HH */
