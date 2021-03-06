/*
 * libfscp - C++ portable OpenSSL cryptographic wrapper library.
 * Copyright (C) 2010-2011 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libfscp.
 *
 * libfscp is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libfscp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use libfscp in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file hello_request.cpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief A basic hello request class.
 */

#include "hello_request.hpp"

#include <boost/bind.hpp>

namespace fscp
{
	namespace
	{
		bool hello_request_match(boost::shared_ptr<hello_request> _hello_request, uint32_t unique_number, const hello_request::ep_type& target)
		{
			return (_hello_request->unique_number() == unique_number) && (_hello_request->target() == target);
		}
	}

	hello_request::hello_request(boost::asio::io_service& io_service, uint32_t _unique_number, const ep_type& _target, callback_type _callback, boost::posix_time::time_duration _timeout) :
		m_unique_number(_unique_number),
		m_target(_target),
		m_callback(_callback),
		m_birthdate(boost::posix_time::microsec_clock::universal_time()),
		m_timeout_timer(io_service, _timeout),
		m_cancel_status(false),
		m_triggered(false)
	{
		m_timeout_timer.async_wait(boost::bind(&hello_request::handle_timeout, this, boost::asio::placeholders::error));
	}

	void hello_request::handle_timeout(const boost::system::error_code& error)
	{
		if (!error)
		{
			trigger();
		}
	}

	hello_request_list::iterator find_hello_request(hello_request_list& _hello_request_list, uint32_t unique_number, const hello_request::ep_type& target)
	{
		return std::find_if(_hello_request_list.begin(), _hello_request_list.end(), boost::bind(&hello_request_match, _1, unique_number, target));
	}

	void erase_expired_hello_requests(hello_request_list& _hello_request_list)
	{
		_hello_request_list.erase(std::remove_if(_hello_request_list.begin(), _hello_request_list.end(), boost::bind(&hello_request::expired, _1)), _hello_request_list.end());
	}
}
