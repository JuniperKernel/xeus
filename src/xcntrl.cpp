/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "xcntrl.hpp"
#include "zmq_addon.hpp"
#include "xmiddleware.hpp"
#include <iterator>

namespace xeus
{

    xcntrl::xcntrl(zmq::context_t& context,
                   const std::string& transport,
                   const std::string& ip,
                   const std::string& port)
        : m_cntrl(context, zmq::socket_type::router),
          m_controller(context, zmq::socket_type::sub)
    {
        m_cntrl.setsockopt(ZMQ_LINGER, get_socket_linger());
        m_cntrl.bind(get_end_point(transport, ip, port));
        m_controller.connect(get_controller_end_point());
        m_controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    void register_xserver(server_impl* xs) { m_xs = xs; }

    void xheartbeat::run()
    {
        zmq::pollitem_t items[] = {
            { m_cntrl, 0, ZMQ_POLLIN, 0 },
            { m_controller, 0, ZMQ_POLLIN, 0 }
        };

        while (true)
        {
            zmq::poll(&items[0], 2, -1);

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmq::multipart_t wire_msg;
                wire_msg.recv(m_cntrl);
                m_xs->notify_control_listener(wire_msg);
            }

            if (items[1].revents & ZMQ_POLLIN)
            {
                // stop or restart message
                break;
            }
        }
    }

}
