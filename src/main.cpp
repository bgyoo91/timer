#include <iostream>

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/local_time/posix_time_zone.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::date_time;
namespace gregorian = boost::gregorian;
namespace posix_time = boost::posix_time;
namespace asio = boost::asio;

// local(KST) to UTC conversion
typedef boost::date_time::local_adjustor<posix_time::ptime, 9, posix_time::no_dst> kst;

void recording(const boost::system::error_code&, asio::deadline_timer*);

int main(int argc, char* argv[])
{
    asio::io_service io_service;
    asio::deadline_timer timer(io_service);

    std::string starting_time = "14:11:00";
    posix_time::time_duration user_time = posix_time::duration_from_string(starting_time);

    gregorian::date today = gregorian::day_clock::local_day();
    posix_time::ptime expiration_time (today, user_time);
    expiration_time = kst::local_to_utc(expiration_time);
    timer.expires_at(expiration_time);

    std::cerr << expiration_time << std::endl;
    std::cerr << expiration_time.zone_name(true) << std::endl;

    timer.async_wait(boost::bind(recording, asio::placeholders::error, &timer));
    io_service.run();
}

void recording(const boost::system::error_code& e, asio::deadline_timer *dt)
{
    static posix_time::ptime time = dt->expires_at();
    std::cout << "Date: " << dt->expires_at() << std::endl;
    
    auto sub = dt->expires_at().time_of_day() - time.time_of_day();
    if(sub.minutes() >= posix_time::duration_from_string("00:01:00").minutes())
    {
        std::cout << "1 minutes" << std::endl;
        return;
    }
    // while(1)
    // {

    // }
    
    // @ adding a day offset
    // @ reference: https://www.boost.org/doc/libs/1_55_0/doc/html/date_time/posix_time.html
    // (*expiration_time) += gregorian::days(1);
    dt->expires_at(dt->expires_at() + posix_time::seconds(10));
    dt->async_wait(boost::bind(recording, boost::asio::placeholders::error, dt));
}
