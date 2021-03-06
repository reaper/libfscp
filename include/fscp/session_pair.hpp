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
 * \file session_pair.hpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief An session pair class.
 */

#ifndef FSCP_SESSION_PAIR_HPP
#define FSCP_SESSION_PAIR_HPP

#include "session_store.hpp"

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace fscp
{
	/**
	 * \brief An session pair class.
	 */
	class session_pair
	{
		public:

			/**
			 * \brief Create an empty session_pair.
			 */
			session_pair();

			/**
			 * \brief Check if the session_pair has a local session.
			 * \return true if the session_pair has a local session.
			 */
			bool has_local_session() const;

			/**
			 * \brief Check if the session_pair has a remote session.
			 * \return true if the session_pair has a remote session.
			 */
			bool has_remote_session() const;

			/**
			 * \brief Get the local session.
			 * \return The local session.
			 * \warning If has_local_session() is false, the behavior is undefined.
			 */
			session_store& local_session();

			/**
			 * \brief Get the local session.
			 * \return The local session.
			 * \warning If has_local_session() is false, the behavior is undefined.
			 */
			const session_store& local_session() const;

			/**
			 * \brief Get the remote session.
			 * \return The remote session.
			 * \warning If has_remote_session() is false, the behavior is undefined.
			 */
			session_store& remote_session();

			/**
			 * \brief Get the remote session.
			 * \return The remote session.
			 * \warning If has_remote_session() is false, the behavior is undefined.
			 */
			const session_store& remote_session() const;

			/**
			 * \brief Renew the local session, if it is old or if the renewal is forced.
			 * \param session_number The requested session number. Set it to a higher value than the current session number to force the change.
			 * \return true if the session was effectively renewed. Should be true if force is set to true.
			 */
			bool renew_local_session(session_store::session_number_type session_number);

			/**
			 * \brief Set the remote session.
			 * \param session The remote session.
			 */
			void set_remote_session(const session_store& session);

			/**
			 * \brief Clear the remote session.
			 */
			bool clear_remote_session();

			/**
			 * \brief Check if the session has timed out.
			 * \param timeout The timeout value.
			 * \return true if the session has timed out, false otherwise.
			 */
			bool has_timed_out(const boost::posix_time::time_duration& timeout) const;

			/**
			 * \brief Keep the session alive.
			 */
			void keep_alive();

			/**
			 * \brief Get the local challenge.
			 * \return The local challenge.
			 */
			const challenge_type& local_challenge() const
			{
				return m_local_challenge;
			}

			/**
			 * \brief Generate a new local challenge.
			 * \return The local challenge.
			 */
			const challenge_type& generate_local_challenge();

			/**
			 * \brief Get the remote challenge.
			 * \return The remote challenge.
			 */
			const challenge_type& remote_challenge() const
			{
				return m_remote_challenge;
			}

			/**
			 * \brief Set the remote challenge.
			 * \param challenge The remote challenge.
			 */
			void set_remote_challenge(const challenge_type& challenge)
			{
				m_remote_challenge = challenge;
			}

		private:

			boost::optional<session_store> m_local_session;
			boost::optional<session_store> m_remote_session;
			boost::posix_time::ptime m_last_sign_of_life;
			challenge_type m_local_challenge;
			challenge_type m_remote_challenge;
	};

	inline session_pair::session_pair() :
		m_last_sign_of_life(boost::posix_time::microsec_clock::local_time())
	{
	}

	inline bool session_pair::has_local_session() const
	{
		return static_cast<bool>(m_local_session);
	}

	inline bool session_pair::has_remote_session() const
	{
		return static_cast<bool>(m_remote_session);
	}

	inline session_store& session_pair::local_session()
	{
		return *m_local_session;
	}

	inline const session_store& session_pair::local_session() const
	{
		return *m_local_session;
	}

	inline session_store& session_pair::remote_session()
	{
		return *m_remote_session;
	}

	inline const session_store& session_pair::remote_session() const
	{
		return *m_remote_session;
	}

	inline bool session_pair::clear_remote_session()
	{
		bool cleared = has_remote_session();

		m_remote_session.reset();

		return cleared;
	}

	inline bool session_pair::has_timed_out(const boost::posix_time::time_duration& timeout) const
	{
		return (boost::posix_time::microsec_clock::local_time() > m_last_sign_of_life + timeout);
	}

	inline void session_pair::keep_alive()
	{
		m_last_sign_of_life = boost::posix_time::microsec_clock::local_time();
	}
}

#endif /* FSCP_SESSION_PAIR_HPP */
