
//   Copyright Christopher M. Kohlhoff, Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_ASIO_ROUND_ROBIN_HPP
#define BOOST_FIBERS_ASIO_ROUND_ROBIN_HPP

#include <deque>
#include <utility>

#include <boost/assert.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/config.hpp>
#include <boost/thread/lock_types.hpp> 

#include <boost/fiber/algorithm.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_base.hpp>
#include <boost/fiber/detail/main_notifier.hpp>
#include <boost/fiber/detail/notify.hpp>
#include <boost/fiber/detail/spinlock.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {
namespace asio {

class BOOST_FIBERS_DECL round_robin : public algorithm
{
private:
    struct schedulable
    {
        detail::fiber_base::ptr_t       f;
        clock_type::time_point          tp;
        boost::asio::io_service::work   w;

        schedulable( detail::fiber_base::ptr_t const& f_,
                     clock_type::time_point const& tp_,
                     boost::asio::io_service::work const& w_) :
            f( f_), tp( tp_), w( w_)
        { BOOST_ASSERT( f); }

        schedulable( detail::fiber_base::ptr_t const& f_,
                     boost::asio::io_service::work const& w_) :
            f( f_), tp( (clock_type::time_point::max)() ), w( w_)
        { BOOST_ASSERT( f); }
    };

    typedef std::deque< schedulable >                   wqueue_t;

    boost::asio::io_service &   io_svc_;
    detail::fiber_base::ptr_t   active_fiber_;
    wqueue_t                    wqueue_;
    detail::main_notifier       mn_;

    void evaluate_( detail::fiber_base::ptr_t const&);

public:
    round_robin( boost::asio::io_service & svc) BOOST_NOEXCEPT;

    ~round_robin() BOOST_NOEXCEPT;

    void spawn( detail::fiber_base::ptr_t const&);

    void priority( detail::fiber_base::ptr_t const&, int) BOOST_NOEXCEPT;

    void join( detail::fiber_base::ptr_t const&);

    detail::fiber_base::ptr_t active() BOOST_NOEXCEPT
    { return active_fiber_; }

    bool run();

    void wait( unique_lock< detail::spinlock > &);
    bool wait_until( clock_type::time_point const&,
                     unique_lock< detail::spinlock > &);

    void yield();

    detail::fiber_base::id get_main_id()
    { return detail::fiber_base::id( detail::main_notifier::make_pointer( mn_) ); }

    detail::notify::ptr_t get_main_notifier()
    { return detail::notify::ptr_t( new detail::main_notifier() ); }
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_ASIO_ROUND_ROBIN_HPP
