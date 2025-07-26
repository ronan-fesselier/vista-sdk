/**
 * @file PrecisionTimer.h
 * @brief High-precision timer utility for unit tests
 */

#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace dnv::vista::sdk::tests
{
	/**
	 * @brief High-precision timer for measuring test execution times in nanoseconds/microseconds
	 */
	class PrecisionTimer
	{
	public:
		/**
		 * @brief Start timing
		 */
		void start()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief Stop timing and return elapsed time
		 * @return Elapsed time in nanoseconds
		 */
		std::chrono::nanoseconds stop()
		{
			auto end = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>( end - m_start );
		}

		/**
		 * @brief Stop timing and print elapsed time with custom message
		 * @param message Custom message to print
		 */
		void stopAndPrint( const std::string& message = "Elapsed time" )
		{
			auto elapsed = stop();
			auto ns = elapsed.count();

			if ( ns < 1000 )
			{
				std::cout << "[TIMING] " << message << ": " << ns << " ns" << std::endl;
			}
			else if ( ns < 1000000 )
			{
				std::cout << "[TIMING] " << message << ": " << ( ns / 1000.0 ) << " μs" << std::endl;
			}
			else
			{
				std::cout << "[TIMING] " << message << ": " << ( ns / 1000000.0 ) << " ms" << std::endl;
			}
		}

	private:
		std::chrono::high_resolution_clock::time_point m_start;
	};

	/**
	 * @brief RAII timer that automatically prints timing on destruction
	 */
	class ScopedTimer
	{
	public:
		explicit ScopedTimer( const std::string& message ) : m_message( message )
		{
			m_timer.start();
		}

		~ScopedTimer()
		{
			m_timer.stopAndPrint( m_message );
		}

	private:
		PrecisionTimer m_timer;
		std::string m_message;
	};
}

#define PRECISION_TIME( message ) dnv::vista::sdk::tests::ScopedTimer _timer( message )
