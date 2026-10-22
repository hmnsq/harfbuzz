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

#నిర్వచితం_కాకపోతే HB_MUTEX_HH
#నిర్వచించు HB_MUTEX_HH

#అన్తర్భావ "hb.hh"


/* mutex */

/* We need external help for these */

#ఒకవేళ defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_తాళం) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_ముగింపు)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#లేదావేళ !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#అన్తర్భావ <pthread.h>
రకనిర్వచనం pthread_mutex_t hb_mutex_impl_t;
#నిర్వచించు hb_mutex_impl_init(M)	pthread_mutex_init (M, శూన్యసూచిక)
#నిర్వచించు hb_mutex_impl_తాళం(M)	pthread_mutex_lock (M)
#నిర్వచించు hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#నిర్వచించు hb_mutex_impl_ముగింపు(M)	pthread_mutex_destroy (M)


#లేదావేళ !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

రకనిర్వచనం CRITICAL_SECTION hb_mutex_impl_t;
#ఒకవేళ !WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP) && WINAPI_FAMILY_విభజన(WINAPI_విభజన_APP)
#నిర్వచించు hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#లేకపోతే
#నిర్వచించు hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#అయితే_ముగింపు
#నిర్వచించు hb_mutex_impl_తాళం(M)	EnterCriticalSection (M)
#నిర్వచించు hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#నిర్వచించు hb_mutex_impl_ముగింపు(M)	DeleteCriticalSection (M)


#లేదావేళ !defined(HB_NO_MT)

#అన్తర్భావ <mutex>
రకనిర్వచనం std::mutex              hb_mutex_impl_t;
#నిర్వచించు hb_mutex_impl_init(M)   HB_STMT_ప్రారంభం { కొత్త (M) hb_mutex_impl_t; } HB_STMT_చివర
#నిర్వచించు hb_mutex_impl_తాళం(M)   (M)->lock ()
#నిర్వచించు hb_mutex_impl_unlock(M) (M)->unlock ()
#నిర్వచించు hb_mutex_impl_ముగింపు(M) HB_STMT_ప్రారంభం { (M)->~hb_mutex_impl_t(); } HB_STMT_చివర


#లేకపోతే /* defined(HB_NO_MT) */

రకనిర్వచనం పూర్ణాంకం hb_mutex_impl_t;
#నిర్వచించు hb_mutex_impl_init(M)	HB_STMT_ప్రారంభం {} HB_STMT_చివర
#నిర్వచించు hb_mutex_impl_తాళం(M)	HB_STMT_ప్రారంభం {} HB_STMT_చివర
#నిర్వచించు hb_mutex_impl_unlock(M)	HB_STMT_ప్రారంభం {} HB_STMT_చివర
#నిర్వచించు hb_mutex_impl_ముగింపు(M)	HB_STMT_ప్రారంభం {} HB_STMT_చివర


#అయితే_ముగింపు


నిర్మాణం hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) అక్షరం m[పరిమాణం (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (స్థిరరాశి hb_mutex_t &) = తొలగించు;
  hb_mutex_t &క్రియాకారకం= (స్థిరరాశి hb_mutex_t &) = తొలగించు;

#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
  శూన్య init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  శూన్య lock   () { hb_mutex_impl_తాళం   ((hb_mutex_impl_t *) m); }
  శూన్య unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  శూన్య fini   () { hb_mutex_impl_ముగింపు ((hb_mutex_impl_t *) m); }
#సూచన GCC diagnostic తీయి
};

నిర్మాణం hb_తాళం_t
{
  hb_తాళం_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_తాళం_t (hb_mutex_t *mutex_) : mutex (mutex_) { ఒకవేళ (mutex) mutex->lock (); }
  ~hb_తాళం_t () { ఒకవేళ (mutex) mutex->unlock (); }

  hb_తాళం_t (స్థిరరాశి hb_తాళం_t &) = తొలగించు;
  hb_తాళం_t &క్రియాకారకం= (స్థిరరాశి hb_తాళం_t &) = తొలగించు;

  అంతరంగ:
  hb_mutex_t *mutex;
};


#అయితే_ముగింపు /* HB_MUTEX_HH */
