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
 * \file data_message.hpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief A data message class.
 */

#ifndef FSCP_DATA_MESSAGE_HPP
#define FSCP_DATA_MESSAGE_HPP

#include "message.hpp"

#include "constants.hpp"

#include <cryptoplus/pkey/pkey.hpp>

namespace fscp
{
	/**
	 * \brief A data message class.
	 */
	class data_message : public message
	{
		public:

			/**
			 * \brief Write a data message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param channel_number The channel number.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param cleartext The cleartext data.
			 * \param cleartext_len The data length.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes written.
			 */
			static size_t write(void* buf, size_t buf_len, channel_number_type channel_number, session_number_type session_number, sequence_number_type sequence_number, const void* cleartext, size_t cleartext_len, const void* seal_key, size_t seal_key_len, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Write a contact-request message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param hash_list The hash list.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes written.
			 */
			static size_t write_contact_request(void* buf, size_t buf_len, session_number_type session_number, sequence_number_type sequence_number, const hash_list_type& hash_list, const void* seal_key, size_t seal_key_len, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Write a contact message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param contact_map The contact map.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes written.
			 */
			static size_t write_contact(void* buf, size_t buf_len, session_number_type session_number, sequence_number_type sequence_number, const contact_map_type& contact_map, const void* seal_key, size_t seal_key_len, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Write a keep-alive message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param random_len The length of the random content to send.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes written.
			 */
			static size_t write_keep_alive(void* buf, size_t buf_len, session_number_type session_number, sequence_number_type sequence_number, size_t random_len, const void* seal_key, size_t seal_key_len, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Parse the hash list.
			 * \param buf The buffer to parse.
			 * \param buflen The length of the buffer to parse.
			 * \return The hash list.
			 */
			static std::vector<hash_type> parse_hash_list(void* buf, size_t buflen);

			/**
			 * \brief Parse the contact map.
			 * \param buf The buffer to parse.
			 * \param buflen The length of the buffer to parse.
			 * \return The contact map.
			 */
			static contact_map_type parse_contact_map(void* buf, size_t buflen);

			/**
			 * \brief Create a data_message and map it on a buffer.
			 * \param buf The buffer.
			 * \param buf_len The buffer length.
			 *
			 * If the mapping fails, a std::runtime_error is thrown.
			 */
			data_message(const void* buf, size_t buf_len);

			/**
			 * \brief Create a data_message from a message.
			 * \param message The message.
			 */
			data_message(const message& message);

			/**
			 * \brief Get the sequence number.
			 * \return The sequence number.
			 */
			sequence_number_type sequence_number() const;

			/**
			 * \brief Get the ciphertext.
			 * \return The ciphertext.
			 */
			const uint8_t* ciphertext() const;

			/**
			 * \brief Get the ciphertext size.
			 * \return The ciphertext size.
			 */
			size_t ciphertext_size() const;

			/**
			 * \brief Get the hmac seal.
			 * \return The hmac seal.
			 */
			const uint8_t* hmac() const;

			/**
			 * \brief Get the hmac seal size.
			 * \return The hmac seal size.
			 */
			size_t hmac_size() const;

			/**
			 * \brief Check if the seal matches with a given seal key.
			 * \param tmp A temporary buffer to use.
			 * \param tmp_len The temporary buffer length. Should be at least 32 bytes long.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \warning If the check fails, an exception is thrown.
			 */
			void check_seal(void* tmp, size_t tmp_len, const void* seal_key, size_t seal_key_len) const;

			/**
			 * \brief Get the clear text data, using a given encryption key.
			 * \param buf The buffer that must receive the data. If buf is NULL, the function returns the expected size of buf.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes deciphered.
			 */
			size_t get_cleartext(void* buf, size_t buf_len, const session_number_type session_number, const void* enc_key, size_t enc_key_len) const;

			/**
			 * \brief Get the clear text data, using a given encryption key.
			 * \param session_number The session number.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The clear text data.
			 */
			template <typename T>
			std::vector<T> get_cleartext(session_number_type session_number, const void* enc_key, size_t enc_key_len) const;

		protected:

			/**
			 * \brief The min length of the body.
			 */
			static const size_t MIN_BODY_LENGTH = sizeof(sequence_number_type);

			/**
			 * \brief Compute and write the initialization vector to a given buffer.
			 * \param buf The buffer the must receive the initialization vector. If buf is NULL, the function returns the expected size of buf.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The count of bytes written.
			 */
			static size_t compute_initialization_vector(void* buf, size_t buf_len, session_number_type session_number, sequence_number_type sequence_number, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Compute and write the initialization vector to a buffer.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \return The initialization vector.
			 */
			template <typename T>
			static std::vector<T> compute_initialization_vector(session_number_type session_number, sequence_number_type sequence_number, const void* enc_key, size_t enc_key_len);

			/**
			 * \brief Write a data message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sequence_number The sequence number.
			 * \param cleartext The cleartext data.
			 * \param cleartext_len The data length.
			 * \param seal_key The seal key.
			 * \param seal_key_len The seal key length.
			 * \param enc_key The encryption key.
			 * \param enc_key_len The encryption key length.
			 * \param type The message type.
			 * \return The count of bytes written.
			 */
			static size_t raw_write(void* buf, size_t buf_len, session_number_type session_number, sequence_number_type sequence_number, const void* cleartext, size_t cleartext_len, const void* seal_key, size_t seal_key_len, const void* enc_key, size_t enc_key_len, message_type type);

		private:

			void check_format() const;
	};

	inline sequence_number_type data_message::sequence_number() const
	{
		return ntohl(buffer_tools::get<sequence_number_type>(payload(), 0));
	}

	inline const uint8_t* data_message::ciphertext() const
	{
		return payload() + sizeof(sequence_number_type);
	}

	inline size_t data_message::ciphertext_size() const
	{
		return length() - sizeof(sequence_number_type) - hmac_size();
	}

	inline const uint8_t* data_message::hmac() const
	{
		return payload() + sizeof(sequence_number_type) + ciphertext_size();
	}

	inline size_t data_message::hmac_size() const
	{
		return cryptoplus::hash::message_digest_algorithm(MESSAGE_DIGEST_ALGORITHM).result_size() / 2;
	}

	template <typename T>
	inline std::vector<T> data_message::get_cleartext(session_number_type session_number, const void* enc_key, size_t enc_key_len) const
	{
		std::vector<T> result(get_cleartext(NULL, 0, session_number, enc_key, enc_key_len));

		result.resize(get_cleartext(&result[0], result.size(), session_number, enc_key, enc_key_len));

		return result;
	}

	template <typename T>
	inline std::vector<T> data_message::compute_initialization_vector(session_number_type session_number, sequence_number_type sequence_number, const void* enc_key, size_t enc_key_len)
	{
		std::vector<T> result(compute_initialization_vector(NULL, 0, session_number, sequence_number, enc_key, enc_key_len));

		result.resize(compute_initialization_vector(&result[0], result.size(), session_number, sequence_number, enc_key, enc_key_len));

		return result;
	}
}

#endif /* FSCP_DATA_MESSAGE_HPP */
