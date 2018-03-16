/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <array>
#include <cstddef>
#include "xeus/xauthentication.hpp"
#include "xeus/make_unique.hpp"
#include "xstring_utils.hpp"
#include "brumme/sha256.h"
#include "brumme/hmac.h"

namespace xeus {

  static std::string read_str(const zmq::message_t& msg) {
    std::stringstream ss;
    const char* buf = msg.data<const char>();
    size_t buflen = msg.size();
    for(size_t i=0; i<buflen; ++i)
      ss << static_cast<char>(buf[i]);
    return ss.str();
  }

  static void read_json(const zmq::message_t& msg, std::stringstream& data) {
    std::string s = read_str(msg);
    data << s;
  }

  static std::string read_msg_parts(
    const zmq::message_t& header,
    const zmq::message_t& parent_header,
    const zmq::message_t& meta_data,
    const zmq::message_t& content) {
      std::stringstream data;
      read_json(header, data);
      read_json(parent_header,data);
      read_json(meta_data, data);
      read_json(content, data);
      return data.str();
  }

  class xauthentication_impl : public xauthentication {

  public:
    explicit xauthentication_impl(const std::string& key);
    virtual ~xauthentication_impl() = default;

  private:
    zmq::message_t sign_impl(
      const zmq::message_t& header,
      const zmq::message_t& parent_header,
      const zmq::message_t& meta_data,
      const zmq::message_t& content) const override;

    bool verify_impl(
      const zmq::message_t& signature,
      const zmq::message_t& header,
      const zmq::message_t& parent_header,
      const zmq::message_t& meta_data,
      const zmq::message_t& content) const override;

  private:
    mutable std::string m_key;
  };

  class no_xauthentication : public xauthentication {
    public:
      no_xauthentication() = default;
      virtual ~no_xauthentication() = default;
    private:
      zmq::message_t sign_impl(
        const zmq::message_t& header,
        const zmq::message_t& parent_header,
        const zmq::message_t& meta_data,
        const zmq::message_t& content) const override;

      bool verify_impl(
        const zmq::message_t& signature,
        const zmq::message_t& header,
        const zmq::message_t& parent_header,
        const zmq::message_t& meta_data,
        const zmq::message_t& content) const override;
  };

  zmq::message_t xauthentication::sign(
    const zmq::message_t& header,
    const zmq::message_t& parent_header,
    const zmq::message_t& meta_data,
    const zmq::message_t& content) const {
      return sign_impl(header, parent_header, meta_data, content);
  }

  bool xauthentication::verify(
    const zmq::message_t& signature,
    const zmq::message_t& header,
    const zmq::message_t& parent_header,
    const zmq::message_t& meta_data,
    const zmq::message_t& content) const {
      return verify_impl(signature, header, parent_header, meta_data, content);
  }

    const std::string sha256_scheme = "hmac-sha256";

    std::unique_ptr<xauthentication> make_xauthentication(
      const std::string& scheme,
      const std::string& key) {
        if( scheme==sha256_scheme)
          return ::xeus::make_unique<xauthentication_impl>(key);
        return ::xeus::make_unique<no_xauthentication>();
    }

    xauthentication_impl::xauthentication_impl(const std::string& key) {
      m_key = key;
    }

    zmq::message_t xauthentication_impl::sign_impl(
      const zmq::message_t& header,
      const zmq::message_t& parent_header,
      const zmq::message_t& meta_data,
      const zmq::message_t& content) const {
        std::string data = read_msg_parts(header, parent_header, meta_data, content);
        std::string hex_sig = hmac<SHA256>(data, m_key);
        return zmq::message_t(hex_sig.begin(), hex_sig.end());
    }

    bool xauthentication_impl::verify_impl(
      const zmq::message_t& signature,
      const zmq::message_t& header,
      const zmq::message_t& parent_header,
      const zmq::message_t& meta_data,
      const zmq::message_t& content) const {
        std::string data = read_msg_parts(header, parent_header, meta_data, content);
        std::string hmac2dig = hmac<SHA256>(data, m_key);
        std::string hmac_hex = read_str(signature);
        return hmac2dig==hmac_hex;
    }

    zmq::message_t no_xauthentication::sign_impl(
      const zmq::message_t& /*header*/,
      const zmq::message_t& /*parent_header*/,
      const zmq::message_t& /*meta_data*/,
      const zmq::message_t& /*content*/) const {
        return zmq::message_t(0);
    }

    bool no_xauthentication::verify_impl(
      const zmq::message_t& /*signature*/,
      const zmq::message_t& /*header*/,
      const zmq::message_t& /*parent_header*/,
      const zmq::message_t& /*meta_data*/,
      const zmq::message_t& /*content*/) const {
        return true;
    }
}
