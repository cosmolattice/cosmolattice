#ifndef TEMPLAT_PARALLEL_THREADS_THREADPOOL_H
#define TEMPLAT_PARALLEL_THREADS_THREADPOOL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/threads/threadsettings.h"

namespace TempLat {


    /** \brief A class which
     * \todo That's where the threading is defined. We have in mind some sort of simple openMP-like construct, inspired by
     * http://www.thesaguaros.com/openmp-style-constructs-in-c11.html . Not implemented.
     * 
     * 
     * Unit test: make test-threadpool
     **/

    class ThreadPool {
    public:
        /* Put public methods here. These should change very little over time. */
        ThreadPool() {
        }
        template<typename F>
        void addThread(F&& f)
        {
            mThreads.push_back(std::thread(std::forward<F>(f)));
        }

        void join() {
            for_each(mThreads.begin(), mThreads.end(),
                     [] (std::thread& t) {t.join();});
        }

        size_t size() const
        {
            return mThreads.size();
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    std::vector<std::thread> mThreads;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/parallel/threads/threadpool_test.h"
#endif


#endif
