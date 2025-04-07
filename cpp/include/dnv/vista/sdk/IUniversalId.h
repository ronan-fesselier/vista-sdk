#pragma once

#include "ImoNumber.h"
#include "LocalId.h"

namespace dnv::vista::sdk
{
	/**
	 * Interface representing a universal identifier.
	 */
	class IUniversalId
	{
	public:
		virtual ~IUniversalId() = default;

		/**
		 * Gets the IMO number.
		 * @return The IMO number.
		 */
		virtual const ImoNumber& GetImoNumber() const = 0;

		/**
		 * Gets the local ID.
		 * @return The local ID.
		 */
		virtual const LocalId& GetLocalId() const = 0;
	};
}
