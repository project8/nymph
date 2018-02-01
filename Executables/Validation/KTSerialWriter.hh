/*
 *	KTSerialWriter.hh
 *
 *	Created on: Feb 1, 2018
 *		Author: E Zayas
 *
*/

#ifndef KTSERIALWRITER_HH_
#define KTSERIALWRITER_HH_

#include "KTData.hh"
#include "KTProcessor.hh"
#include "KTSlot.hh"

#include "cereal/archives/json.hpp"
#include <fstream>

namespace Nymph
{
	class KTSerialWriter : public KTProcessor
	{
		public:
			KTSerialWriter( const std::string& name = "serial-writer" );
			virtual ~KTSerialWriter();

		private:
			std::string fFileName;
			std::ofstream* fStreamOutPtr;
			cereal::JSONOutputArchive* fArchiveOutPtr;

		public:
			bool Configure( const scarab::param_node* node );

			std::string GetFileName() const;
			void SetFileName( std::string file );

			std::ofstream* GetStreamOutPtr() const;
			void SetStreamOutPtr( std::ofstream stream );

			cereal::JSONOutputArchive* GetArchiveOutPtr() const;
			void SetArchiveOutPtr( cereal::JSONOutputArchive archive );

			template< class XDataType >
			void SlotFunction( const XDataType& data );
	};

	inline std::string KTSerialWriter::GetFileName() const
	{
		return fFileName;
	}

	inline void KTSerialWriter::SetFileName( std::string file )
	{
		fFileName = file;
		return;
	}

	inline std::ofstream* KTSerialWriter::GetStreamOutPtr() const
	{
		return fStreamOutPtr;
	}

	inline void KTSerialWriter::SetStreamOutPtr( std::ofstream stream )
	{
		fStreamOutPtr = &stream;
		return;
	}

	inline cereal::JSONOutputArchive* KTSerialWriter::GetArchiveOutPtr() const
	{
		return fArchiveOutPtr;
	}

	inline void KTSerialWriter::SetArchiveOutPtr( cereal::JSONOutputArchive archive )
	{
		fArchiveOutPtr = &archive;
		return;
	}

} // namespace Nymph

#endif // KTSERIALWRITER_HH_
