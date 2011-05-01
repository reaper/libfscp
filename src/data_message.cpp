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
 * \file data_message.cpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief A data message class.
 */

#include "data_message.hpp"

#include <cryptoplus/cipher/cipher_context.hpp>
#include <cryptoplus/cipher/cipher_stream.hpp>
#include <cryptoplus/hash/hmac.hpp>
#include <cassert>
#include <stdexcept>

namespace fscp
{
	size_t data_message::get_ciphered_data_size(size_t cleartext_size)
	{
		return ((cleartext_size + 1) / BLOCK_SIZE) * BLOCK_SIZE;
	}

	size_t data_message::write(void* buf, size_t buf_len, sequence_number_type _sequence_number, const void* _data, size_t data_len, const void* sig_key, size_t sig_key_len, const void* enc_key, size_t enc_key_len, const void* iv, size_t iv_len)
	{
		assert(sig_key);
		assert(enc_key);
		assert(iv);
		assert(sig_key_len == KEY_SIZE);
		assert(enc_key_len == KEY_SIZE);
		assert(iv_len == IV_SIZE);

		cryptoplus::cipher::cipher_stream cipher_stream(data_len + BLOCK_SIZE);
		cipher_stream.initialize(cryptoplus::cipher::cipher_algorithm(NID_aes_256_cbc), cryptoplus::cipher::cipher_stream::encrypt, enc_key, iv);
		cipher_stream.append(_data, data_len);
		cipher_stream.finalize();

		const size_t payload_len = MIN_BODY_LENGTH + cipher_stream.result().size();

		if (buf_len < HEADER_LENGTH + payload_len)
		{
			throw std::runtime_error("buf_len");
		}

		buffer_tools::set<sequence_number_type>(buf, HEADER_LENGTH, htonl(_sequence_number));
		buffer_tools::set<uint16_t>(buf, HEADER_LENGTH + sizeof(sequence_number_type), htons(static_cast<uint16_t>(cipher_stream.result().size())));
		std::memcpy(static_cast<uint8_t*>(buf) + HEADER_LENGTH + sizeof(sequence_number_type) + sizeof(uint16_t), &cipher_stream.result()[0], cipher_stream.result().size());

		cryptoplus::hash::hmac(
		    static_cast<uint8_t*>(buf) + HEADER_LENGTH + sizeof(sequence_number_type) + sizeof(uint16_t) + cipher_stream.result().size(),
		    HMAC_SIZE,
		    sig_key,
		    sig_key_len,
		    static_cast<uint8_t*>(buf) + HEADER_LENGTH,
		    sizeof(sequence_number_type) + sizeof(uint16_t) + cipher_stream.result().size(),
		    cryptoplus::hash::message_digest_algorithm(NID_sha256)
		);

		message::write(buf, buf_len, CURRENT_PROTOCOL_VERSION, MESSAGE_TYPE_DATA, payload_len);

		return HEADER_LENGTH + payload_len;
	}

	data_message::data_message(const void* buf, size_t buf_len) :
		message(buf, buf_len)
	{
		check_format();
	}

	data_message::data_message(const message& _message) :
		message(_message)
	{
		check_format();
	}

	void data_message::check_format() const
	{
		if (length() < MIN_BODY_LENGTH)
		{
			throw std::runtime_error("bad message length");
		}

		if (length() != MIN_BODY_LENGTH + data_size())
		{
			throw std::runtime_error("bad message length");
		}
	}

	void data_message::check_signature(const void* sig_key, size_t sig_key_len) const
	{
		assert(sig_key);
		assert(sig_key_len == KEY_SIZE);

		std::vector<uint8_t> _hmac = cryptoplus::hash::hmac<uint8_t>(
		                                 sig_key,
		                                 sig_key_len,
		                                 payload(),
		                                 sizeof(sequence_number_type) + sizeof(uint16_t) + data_size(),
		                                 cryptoplus::hash::message_digest_algorithm(NID_sha256)
		                             );

		if ((_hmac.size() != hmac_size()) || (std::memcmp(hmac(), &_hmac[0], _hmac.size()) != 0))
		{
			throw std::runtime_error("hmac mismatch");
		}
	}

	size_t data_message::get_cleartext(void* buf, size_t buf_len, const void* enc_key, size_t enc_key_len, const void* iv, size_t iv_len) const
	{
		assert(enc_key);
		assert(iv);
		assert(enc_key_len == KEY_SIZE);
		assert(iv_len == IV_SIZE);

		if (buf)
		{
			cryptoplus::cipher::cipher_context cipher_context;
			cipher_context.initialize(cryptoplus::cipher::cipher_algorithm(NID_aes_256_cbc), cryptoplus::cipher::cipher_context::decrypt, enc_key, iv);
			size_t cnt = cipher_context.update(buf, buf_len, data(), data_size());
			cnt += cipher_context.finalize(static_cast<uint8_t*>(buf) + cnt, buf_len - cnt);

			return cnt;
		}
		else
		{
			return data_size();
		}
	}
}