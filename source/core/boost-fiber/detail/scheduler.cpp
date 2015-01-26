//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/detail/scheduler.hpp"

#include <boost/assert.hpp>

#include <boost/fiber/fiber_manager.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

thread_local_ptr< fiber_manager > scheduler::instance_;

void
scheduler::replace( sched_algorithm * other)
{
    BOOST_ASSERT( other);

    fm_set_sched_algo( other);
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
