#include "TUtil/Formatter.h"

#include <cstring>

namespace TUtil {

	const char Formatter::UPPER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	const char Formatter::LOWER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	FormatFile Print::STDOUT = stdout;
	FormatFile Print::STDERR = stderr;


	const char* Formatter::c_str()
	{
		//ES_ASSERT(m_Offset < m_Capacity, "Formatter has no space for the null termination character! "
		//								 "m_Offset should always save space for the null byte!");
		m_Buf[m_Offset] = '\0';
		return m_Buf;
	}


	//TODO better handling of signed vs unsigned char with regard to u8
	Formatter& Formatter::operator<<(char value)
	{
		//Interpret as a character
		PrintChar(static_cast<char>(value));
		return *this;
	}

#ifdef TUTIL_SIGNED_CHAR
	Formatter& Formatter::operator<<(std::uint8_t value)
	{
		//Interpret as a single byte
		PrintUnsignedInteger<std::uint8_t>(value);
		return *this;
	}
#endif

	Formatter& Formatter::operator<<(std::uint16_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(std::int16_t value)
	{
		PrintSignedInteger(value);

		return *this;
	}

	Formatter& Formatter::operator<<(std::uint32_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(std::int32_t value)
	{
		PrintSignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(std::uint64_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(std::int64_t value)
	{
		PrintSignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(float value)
	{
		*this << "Cannot print floats yet...";
		return *this;
	}

	Formatter& Formatter::operator<<(double value)
	{
		*this << "Cannot print doubles yet...";
		return *this;
	}


	Formatter& Formatter::operator<<(const char* value)
	{
		size_t bytes = std::min(std::strlen(value), m_Capacity - m_Offset - 1);
		std::memcpy(m_Buf + m_Offset, value, bytes);
		m_Offset += bytes;
		return *this;
	}

	Formatter& Formatter::operator<<(const std::string& value)
	{
		*this << value.c_str();
		return *this;
	}

	FormatFile& FormatFile::WriteImpl(char value)
	{
		TryFlush(1);
		m_Wrapper << value;
		return *this;
	}
#ifdef TUTIL_SIGNED_CHAR
	FormatFile& FormatFile::WriteImpl(std::uint8_t value)
	{
		TryFlush(1);
		m_Wrapper << value;
		return *this;
	}
#endif

	FormatFile& FormatFile::WriteImpl(std::uint16_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(std::int16_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(std::uint32_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(std::int32_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(std::uint64_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(std::int64_t value)
	{
		WriteIntegral(value);
		return *this;
	}

	FormatFile& FormatFile::WriteImpl(float value)
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "%f", value);
		WriteImpl(buf);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(double value)
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "%f", value);
		WriteImpl(buf);
		return *this;
	}



	FormatFile& FormatFile::WriteImpl(const std::string& value)
	{
		WriteStringImpl(value.c_str(), value.size());
		return *this;
	}

	FormatFile& FormatFile::WriteImpl(const char* value)
	{
		WriteStringImpl(value, strlen(value));
		return *this;
	}

	void FormatFile::WriteStringImpl(const char* value, std::size_t length)
	{
		if (length >= m_Wrapper.Capacity() / 2)
		{
			//flush the current data and then print the string directly
			Flush();
			std::size_t bytesWritten = fwrite(m_Buf, 1, m_Wrapper.Size(), m_File);
			//ES_ASSERT(bytesWritten == length, "Failed to write to STDOut");
			Flush();//fsync
		}
		else
		{
			//If it is a small string make sure we have enough space then buffer it
			TryFlush(length);
			m_Wrapper.Write(value);
		}
	}

	void FormatFile::TryFlush(std::size_t requiredBytes)
	{
		if (requiredBytes + m_Wrapper.Size() >= m_Wrapper.Capacity())
		{
			if (m_Wrapper.Size())
			{
				std::size_t bytesWritten = fwrite(m_Buf, 1, m_Wrapper.Size(), m_File);
				//ES_ASSERT(bytesWritten == m_Wrapper.Size(), "Failed to write to STDOut");
				m_Wrapper.Clear();
			}

			fflush(m_File);
		}
	}

}
