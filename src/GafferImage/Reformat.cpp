//////////////////////////////////////////////////////////////////////////
//  
//  Copyright (c) 2013, Image Engine Design Inc. All rights reserved.
//  Copyright (c) 2013, Luke Goddard. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//  
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//  
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//  
//      * Neither the name of Image Engine Design nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  
//////////////////////////////////////////////////////////////////////////

#include "GafferImage/Reformat.h"
#include "GafferImage/Scale.h"
#include "GafferImage/Sampler.h"

using namespace Gaffer;
using namespace GafferImage;
using namespace IECore;
using namespace Imath;

IE_CORE_DEFINERUNTIMETYPED( Reformat );

size_t Reformat::g_firstPlugIndex = 0;

Reformat::Reformat( const std::string &name )
	:	ImageProcessor( name )
{
	storeIndexOfNextChild( g_firstPlugIndex );
	addChild( new FormatPlug( "format" ) );
	addChild( new FilterPlug( "filter" ) );
	addChild( new IntPlug( "mode" ) );
	addChild( new BoolPlug( "centerEnabled" ) );

	addChild( new V2fPlug( "__scale", Gaffer::Plug::Out ) );
	addChild( new V2fPlug( "__origin", Gaffer::Plug::Out ) );
	
	GafferImage::Scale *scale = new GafferImage::Scale( std::string( boost::str( boost::format( "__%sScale" )  % name  ) ) );
	scale->inPlug()->setInput( inPlug() );
	scale->filterPlug()->setInput( filterPlug() );
	scale->enabledPlug()->setInput( enabledPlug() );
	scale->originPlug()->setInput( originPlug() );
	scale->scalePlug()->setInput( scalePlug() );
	addChild( scale );

	outPlug()->channelNamesPlug()->setInput( scale->outPlug()->channelNamesPlug() );
	outPlug()->channelDataPlug()->setInput( scale->outPlug()->channelDataPlug() );
	outPlug()->formatPlug()->setInput( formatPlug() );
}

Reformat::~Reformat()
{
}

GafferImage::FormatPlug *Reformat::formatPlug()
{
	return getChild<FormatPlug>( g_firstPlugIndex );
}

const GafferImage::FormatPlug *Reformat::formatPlug() const
{
	return getChild<FormatPlug>( g_firstPlugIndex );
}

GafferImage::FilterPlug *Reformat::filterPlug()
{
	return getChild<GafferImage::FilterPlug>( g_firstPlugIndex+1 );
}

const GafferImage::FilterPlug *Reformat::filterPlug() const
{
	return getChild<GafferImage::FilterPlug>( g_firstPlugIndex+1 );
}

Gaffer::IntPlug *Reformat::modePlug()
{
	return getChild<Gaffer::IntPlug>( g_firstPlugIndex+2 );
}

const Gaffer::IntPlug *Reformat::modePlug() const
{
	return getChild<Gaffer::IntPlug>( g_firstPlugIndex+2 );
}

Gaffer::BoolPlug *Reformat::centerEnabledPlug()
{
	return getChild<Gaffer::BoolPlug>( g_firstPlugIndex+3 );
}

const Gaffer::BoolPlug *Reformat::centerEnabledPlug() const
{
	return getChild<Gaffer::BoolPlug>( g_firstPlugIndex+3 );
}

Gaffer::V2fPlug *Reformat::scalePlug()
{
	return getChild<V2fPlug>( g_firstPlugIndex+4 );
}

const Gaffer::V2fPlug *Reformat::scalePlug() const
{
	return getChild<V2fPlug>( g_firstPlugIndex+4 );
}

Gaffer::V2fPlug *Reformat::originPlug()
{
	return getChild<V2fPlug>( g_firstPlugIndex+5 );
}

const Gaffer::V2fPlug *Reformat::originPlug() const
{
	return getChild<V2fPlug>( g_firstPlugIndex+5 );
}

GafferImage::Scale *Reformat::scaleNode()
{
	return getChild<Scale>( g_firstPlugIndex+6 );
}

const GafferImage::Scale *Reformat::scaleNode() const
{
	return getChild<Scale>( g_firstPlugIndex+6 );
}

void Reformat::affects( const Gaffer::Plug *input, AffectedPlugsContainer &outputs ) const
{
	ImageProcessor::affects( input, outputs );

	if( input == formatPlug() || input == inPlug()->formatPlug() )
	{
		outputs.push_back( scalePlug()->getChild(0) );
		outputs.push_back( scalePlug()->getChild(1) );
		outputs.push_back( originPlug()->getChild(0) );
		outputs.push_back( originPlug()->getChild(1) );
		outputs.push_back( outPlug()->dataWindowPlug() );
	}
	else if ( input == modePlug() )
	{
		outputs.push_back( scalePlug()->getChild(0) );
		outputs.push_back( scalePlug()->getChild(1) );
		outputs.push_back( originPlug()->getChild(0) );
		outputs.push_back( originPlug()->getChild(1) );
		outputs.push_back( outPlug()->dataWindowPlug() );
	}
	else if ( input == centerEnabledPlug() )
	{
		outputs.push_back( scalePlug()->getChild(0) );
		outputs.push_back( scalePlug()->getChild(1) );
		outputs.push_back( originPlug()->getChild(0) );
		outputs.push_back( originPlug()->getChild(1) );
		outputs.push_back( outPlug()->dataWindowPlug() );
	}
}

bool Reformat::enabled() const
{
	if( !ImageProcessor::enabled() )
	{
		return false;
	}

	Format inFormat( inPlug()->formatPlug()->getValue() );
	Format outFormat( formatPlug()->getValue() );
		
	return inFormat != outFormat;
}

void Reformat::compute( ValuePlug *output, const Context *context ) const
{
	Box2i displayWindow( inPlug()->formatPlug()->getValue().getDisplayWindow() );
	if( output == originPlug()->getChild( 0 ) )
	{
		static_cast<FloatPlug *>( output )->setValue( origin().x );
		return;
	}
	else if( output == originPlug()->getChild( 1 ) )
	{
		static_cast<FloatPlug *>( output )->setValue( origin().y );
		return;
	}
	else if( output == scalePlug()->getChild( 0 ) )
	{
		static_cast<FloatPlug *>( output )->setValue( scale().x );
		return;
	}
	else if( output == scalePlug()->getChild( 1 ) )
	{
		static_cast<FloatPlug *>( output )->setValue( scale().y );
		return;
	}
	else if( output == outPlug()->dataWindowPlug() )
	{
		Box2i scaleDataWindow( scaleNode()->outPlug()->dataWindowPlug()->getValue() );
		Box2i scaleDisplayWindow( scaleNode()->outPlug()->formatPlug()->getValue().getDisplayWindow() );
		Box2i displayWindow( formatPlug()->getValue().getDisplayWindow() );
		static_cast<AtomicBox2iPlug *>( output )->setValue( scaleDataWindow );
		return;
	}
	ImageProcessor::compute( output, context );
}

void Reformat::hash( const ValuePlug *output, const Context *context, IECore::MurmurHash &h ) const
{
	ImageProcessor::hash( output, context, h );

	if( output == scalePlug()->getChild( 0 ) )
	{
		h.append( inPlug()->formatPlug()->getValue().getDisplayWindow().size().x );
		h.append( formatPlug()->getValue().getDisplayWindow().size().x );
		h.append( modePlug()->getValue() );
		return;
	}
	else if( output == scalePlug()->getChild( 1 ) )
	{
		h.append( inPlug()->formatPlug()->getValue().getDisplayWindow().size().y );
		h.append( inPlug()->formatPlug()->getValue().getPixelAspect() );
		h.append( formatPlug()->getValue().getDisplayWindow().size().y );
		h.append( formatPlug()->getValue().getPixelAspect() );
		h.append( modePlug()->getValue() );
		return;
	}
	else if( output == originPlug()->getChild( 0 ) )
	{
		h.append( inPlug()->formatPlug()->getValue().getDisplayWindow().min.x );
		h.append( modePlug()->getValue() );
		h.append( centerEnabledPlug()->getValue() );
		return;
	}
	else if( output == originPlug()->getChild( 1 ) )
	{
		h.append( inPlug()->formatPlug()->getValue().getDisplayWindow().min.y );
		h.append( inPlug()->formatPlug()->getValue().getPixelAspect() );
		h.append( modePlug()->getValue() );
		h.append( centerEnabledPlug()->getValue() );
		return;
	}
	else if( output == outPlug()->dataWindowPlug() )
	{
		h.append( scaleNode()->outPlug()->dataWindowPlug()->getValue() );
		h.append( modePlug()->getValue() );
		h.append( centerEnabledPlug()->getValue() );
		return;
	}
}

Imath::V2f Reformat::origin() const
{
	Box2i inDisplayWindow( inPlug()->formatPlug()->getValue().getDisplayWindow() );
	Box2i inDataWindow( inPlug()->dataWindowPlug()->getValue() );
	Box2i outDisplayWindow( formatPlug()->getValue().getDisplayWindow() );
	Imath::V2f s( scale() );	

	if ( centerEnabledPlug()->getValue() )
	{
		const double offsetX = ((inDataWindow.max.x - inDataWindow.min.x) / 2.) * -1.;
		const double offsetY = ((inDataWindow.max.y - inDataWindow.min.y) / 2.) * -1.;
		
		switch ( modePlug()->getValue() )
		{
			case kWidth:
				return Imath::V2f( offsetX, 0. );
				break;
			case kHeight:
				return Imath::V2f( 0., offsetY );
				break;
			case kFit:
				if ( offsetX > offsetY )
				{
					return Imath::V2f( offsetX, 0. );
				}
				else
				{
					return Imath::V2f( 0., offsetY );
				}
				break;
			case kFill:
				if ( offsetX < offsetY )
				{
					return Imath::V2f( offsetX, 0. );
				}
				else
				{
					return Imath::V2f( 0., offsetY );
				}
				break;
			case kDistort:
				return inDisplayWindow.min;
				break;
			default:
				return inDisplayWindow.min;
				break;
		}	
	}
	else
	{	
		return inDisplayWindow.min;
	}
}

Imath::V2f Reformat::scale() const
{
	Box2i outDisplayWindow( formatPlug()->getValue().getDisplayWindow() );
	Box2i displayWindow( inPlug()->formatPlug()->getValue().getDisplayWindow() );
	
	const double scaleWidth = ( outDisplayWindow.size().x + 1. ) / ( displayWindow.size().x + 1. );
	const double scaleHeight = ( outDisplayWindow.size().y + 1. ) / ( displayWindow.size().y + 1. );
	const double scaleFit = std::min( scaleWidth, scaleHeight );
	const double scaleFill = std::max( scaleWidth, scaleHeight );

	switch ( modePlug()->getValue() )
	{
		case kWidth:
			return Imath::V2f( scaleWidth, scaleWidth );
			break;
		case kHeight:
			return Imath::V2f( scaleHeight, scaleHeight );
			break;
		case kFit:
			return Imath::V2f( scaleFit, scaleFit );
			break;
		case kFill:
			return Imath::V2f( scaleFill, scaleFill );
			break;
		case kDistort:
			return Imath::V2f( scaleWidth, scaleHeight );
			break;
		default:
			return Imath::V2f( scaleWidth, scaleHeight );
			break;
	}
}

GafferImage::Format Reformat::computeFormat( const Gaffer::Context *context, const ImagePlug *parent ) const
{
	return inPlug()->formatPlug()->getValue();
}

Imath::Box2i Reformat::computeDataWindow( const Gaffer::Context *context, const ImagePlug *parent ) const
{
	return inPlug()->dataWindowPlug()->getValue();
}

IECore::ConstStringVectorDataPtr Reformat::computeChannelNames( const Gaffer::Context *context, const ImagePlug *parent ) const
{
	return inPlug()->channelNamesPlug()->getValue();
}

IECore::ConstFloatVectorDataPtr Reformat::computeChannelData( const std::string &channelName, const Imath::V2i &tileOrigin, const Gaffer::Context *context, const ImagePlug *parent ) const
{
	IECore::FloatVectorDataPtr outData = inPlug()->channelData( channelName, tileOrigin )->copy();
	return outData;
}

