#pragma once

#include "ImoNumber.h"
#include "LocalId.h"

namespace dnv::vista::sdk
{
	/**
	 * Interface representing a universal identifier.
	 * Provides access to IMO number and local ID components.
	 */
	class IUniversalId
	{
	public:
		/**
		 * Virtual destructor to ensure proper cleanup when deleting derived classes.
		 */
		virtual ~IUniversalId() = default;

		/**
		 * Gets the IMO number component.
		 * @return The IMO number.
		 */
		virtual const ImoNumber& imoNumber() const = 0;

		/**
		 * Gets the local ID component.
		 * @return The local ID.
		 */
		virtual const LocalId& localId() const = 0;
	};
}
