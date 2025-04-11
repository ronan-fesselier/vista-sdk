#include "pch.h"

#include "dnv/vista/sdk/ImoNumber.h"

namespace dnv::vista::sdk
{
	ImoNumber::ImoNumber( int value )
	{
		if ( !IsValid( value ) )
		{
			throw std::invalid_argument( "Invalid IMO number: " + std::to_string( value ) );
		}
		m_value = value;
	}

	ImoNumber::ImoNumber( const std::string& value )
	{
		auto result = TryParse( value );
		if ( !result )
		{
			throw std::invalid_argument( "Invalid IMO number: " + value );
		}
		m_value = static_cast<int>( *result );
	}

	ImoNumber::ImoNumber( int value, bool ) noexcept
		: m_value( value )
	{
	}

	ImoNumber ImoNumber::Parse( const char* value )
	{
		if ( value == nullptr )
			throw std::invalid_argument( "Null IMO number string" );

		return Parse( std::string( value ) );
	}

	ImoNumber ImoNumber::Parse( const std::string& value )
	{
		if ( value.empty() )
			throw std::invalid_argument( "Empty IMO number string" );

		auto result = TryParse( value );
		if ( !result )
			throw std::invalid_argument( "Failed to parse ImoNumber: " + value );

		return *result;
	}

	std::optional<ImoNumber> ImoNumber::TryParse( const std::string& value )
	{
		if ( value.empty() )
			return std::nullopt;

		if ( value.find_first_of( " \t\n\r\f\v" ) != std::string::npos )
			return std::nullopt;

		std::string processed = value;

		if ( processed.size() >= 3 &&
			 ( processed[0] == 'I' || processed[0] == 'i' ) &&
			 ( processed[1] == 'M' || processed[1] == 'm' ) &&
			 ( processed[2] == 'O' || processed[2] == 'o' ) )
		{
			processed = processed.substr( 3 );
		}

		int num = 0;
		try
		{
			num = std::stoi( processed );
		}
		catch ( const std::invalid_argument& )
		{
			return std::nullopt;
		}
		catch ( const std::out_of_range& )
		{
			return std::nullopt;
		}

		if ( num == 0 || !IsValid( num ) )
			return std::nullopt;

		return ImoNumber( num, true );
	}

	// https://en.wikipedia.org/wiki/IMO_number
	// An IMO number is made of the three letters "IMO" followed by a seven-digit number.
	// This consists of a six-digit sequential unique number followed by a check digit.
	// The integrity of an IMO number can be verified using its check digit.
	// This is done by multiplying each of the first six digits by a factor
	// of 2 to 7 corresponding to their position from right to left.
	// The rightmost digit of this sum is the check digit.
	// For example, for IMO 9074729: (9×7) + (0×6) + (7×5) + (4×4) + (7×3) + (2×2) = 139
	bool ImoNumber::IsValid( int imoNumber )
	{
		if ( imoNumber < 1000000 || imoNumber > 9999999 )
			return false;

		int digits[7];
		int temp = imoNumber;
		for ( int i = 6; i >= 0; i-- )
		{
			digits[i] = temp % 10;
			temp /= 10;
		}

		int checkSum = 0;
		for ( int i = 0; i < 6; i++ )
		{
			checkSum += digits[i] * ( 7 - i );
		}

		int calculatedCheckDigit = checkSum % 10;
		int providedCheckDigit = digits[6];

		return providedCheckDigit == calculatedCheckDigit;
	}

	void ImoNumber::GetDigits( int number, unsigned char* digits )
	{
		int current = number;
		int index = 0;

		while ( current > 0 )
		{
			if ( current < 10 )
			{
				digits[index++] = static_cast<unsigned char>( current );
				break;
			}

			int next = current / 10;
			int digit = current - next * 10;
			digits[index++] = static_cast<unsigned char>( digit );
			current = next;
		}
	}

	std::string ImoNumber::ToString() const
	{
		return "IMO" + std::to_string( m_value );
	}

	ImoNumber::operator int() const
	{
		return m_value;
	}

	bool ImoNumber::operator==( const ImoNumber& other ) const
	{
		return m_value == other.m_value;
	}

	std::size_t ImoNumber::Hash::operator()( const ImoNumber& imoNumber ) const noexcept
	{
		return std::hash<int>{}( imoNumber.m_value );
	}
}
