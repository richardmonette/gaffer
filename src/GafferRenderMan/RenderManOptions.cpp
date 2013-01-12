//////////////////////////////////////////////////////////////////////////
//  
//  Copyright (c) 2013, Image Engine Design Inc. All rights reserved.
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
//      * Neither the name of John Haddon nor the names of
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

#include "GafferRenderMan/RenderManOptions.h"

using namespace Imath;
using namespace GafferRenderMan;

IE_CORE_DEFINERUNTIMETYPED( RenderManOptions );

RenderManOptions::RenderManOptions( const std::string &name )
	:	GafferScene::Options( name, Gaffer::Plug::Default )
{
	Gaffer::CompoundDataPlug *options = optionsPlug();
	
	// sampling parameters
	
	/*options->addOptionalMember( "ai:AA_samples", new IECore::IntData( 3 ), "aaSamples", false );
	options->addOptionalMember( "ai:GI_diffuse_samples", new IECore::IntData( 2 ), "giDiffuseSamples", false );
	options->addOptionalMember( "ai:GI_glossy_samples", new IECore::IntData( 2 ), "giGlossySamples", false );
	options->addOptionalMember( "ai:GI_refraction_samples", new IECore::IntData( 2 ), "giRefractionSamples", false );*/

	// searchpath parameters
	
	options->addOptionalMember( "ri:searchpath:shader", new IECore::StringData( "" ), "shaderSearchPath", false );
	options->addOptionalMember( "ri:searchpath:texture", new IECore::StringData( "" ), "textureSearchPath", false );
	options->addOptionalMember( "ri:searchpath:display", new IECore::StringData( "" ), "displaySearchPath", false );
	options->addOptionalMember( "ri:searchpath:archive", new IECore::StringData( "" ), "archiveSearchPath", false );
	options->addOptionalMember( "ri:searchpath:procedural", new IECore::StringData( "" ), "proceduralSearchPath", false );
	
}

RenderManOptions::~RenderManOptions()
{
}