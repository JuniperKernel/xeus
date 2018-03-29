/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XCNTRL_HPP
#define XCNTRL_HPP

#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "xeus/xserver.hpp"

namespace xeus
{

    class xcntrl
    {

    public:

        xcntrl(zmq::context_t& context,
                   const std::string& transport,
                   const std::string& ip,
                   const std::string& port);

        void run();
        void register_xserver(xserver* xs);
        void send_message(zmq::multipart_t& message);

    private:
        zmq::socket_t m_cntrl;
        zmq::socket_t m_controller;
        xserver* m_xs;
    };

}

#endif
