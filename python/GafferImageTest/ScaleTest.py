##########################################################################
#  
#  Copyright (c) 2014, Image Engine Design Inc. All rights reserved.
#  
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
#  
#      * Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#  
#      * Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials provided with
#        the distribution.
#  
#      * Neither the name of John Haddon nor the names of
#        any other contributors to this software may be used to endorse or
#        promote products derived from this software without specific prior
#        written permission.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, OR
#  PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  
##########################################################################

import unittest

import IECore
import Gaffer
import GafferTest
import GafferImage
import os

class ScaleTest( unittest.TestCase ) :

	path = os.path.expandvars( "$GAFFER_ROOT/python/GafferTest/images/" )

	def testDataWindow( self ) :

		read = GafferImage.ImageReader()
		read["fileName"].setValue( os.path.join( self.path, "blueWithDataWindow.100x100.exr" ) )
		
		# Resize the image and check the size of the output data window.
		scale = GafferImage.Scale()

		scale["scale"].setValue( IECore.V2f( 1.25, .5 ) )	
		scale["in"].setInput( read["out"] )
		
		scale["origin"].setValue( IECore.V2f( 0, 0 ) )	
		self.assertEqual( scale["out"]["dataWindow"].getValue(), IECore.Box2i( IECore.V2i( 37, 15 ), IECore.V2i( 99, 39 )  ) )
		
		scale["origin"].setValue( IECore.V2f( 50, 30 ) )	
		self.assertEqual( scale["out"]["dataWindow"].getValue(), IECore.Box2i( IECore.V2i( 25, 30 ), IECore.V2i( 87, 54 )  ) )
	
		# Test the scale with an image that has a negative data window.	
		read["fileName"].setValue( os.path.join( self.path, "checkerWithNegativeDataWindow.200x150.exr" ) )
		scale["origin"].setValue( IECore.V2f( 20, 10 ) )	
		self.assertEqual( scale["out"]["dataWindow"].getValue(), IECore.Box2i( IECore.V2i( -37, -10 ), IECore.V2i( 213, 64 )  ) )
	
	
#	TODO:
#*	It appears that gaffer is displaying checkerWithNegativeDataWindow.200x150.exr incorrectly. Check it out and make sure that it works correctly.
#*	Add an option to the image reader that will 0 offset image formats.

		read["fileName"].setValue( os.path.join( self.path, "checkerWithNegWindows.200x150.exr" ) )
		scale["origin"].setValue( IECore.V2f( -5, -5 ) )	
		
		dw = scale["in"]["format"].getValue().getDisplayWindow()
		print "Input DisplayWindow: %d, %d, %d, %d" % ( dw.min.x, dw.min.y, dw.max.x, dw.max.y ) 
		dw = scale["in"]["dataWindow"].getValue()
		print "Input DataWindow: %d, %d, %d, %d" % ( dw.min.x, dw.min.y, dw.max.x, dw.max.y ) 
		dw = scale["out"]["format"].getValue().getDisplayWindow()
		print "Out DisplayWindow: %d, %d, %d, %d" % ( dw.min.x, dw.min.y, dw.max.x, dw.max.y ) 
		dw = scale["out"]["dataWindow"].getValue()
		print "Out DataWindow: %d, %d, %d, %d" % ( dw.min.x, dw.min.y, dw.max.x, dw.max.y ) 
		
		w = GafferImage.ImageWriter()
		w["fileName"].setValue( "/tmp/scale.exr" )
		w["in"].setInput( scale["out"] )
		w.execute( [ Gaffer.Context() ] )
		
		

		
		read["fileName"].setValue( os.path.join( self.path, "checkerWithNegativeDataWindow.200x150.exr" ) )
		scale["origin"].setValue( IECore.V2f( 20, 10 ) )	
		self.assertEqual( scale["out"]["dataWindow"].getValue(), IECore.Box2i( IECore.V2i( -37, -10 ), IECore.V2i( 213, 64 )  ) )
		
		
	def testEnabled( self ) :
		
		read = GafferImage.ImageReader()
		read["fileName"].setValue( os.path.join( self.path, "blueWithDataWindow.100x100.exr" ) )
		scale = GafferImage.Scale()
		scale["in"].setInput( read["out"] )
		scale["enabled"].setValue( False )

		context = Gaffer.Context()
		context["image:channelName"] = "B"
		context["image:tileOrigin"] = IECore.V2i( GafferImage.ImagePlug.tileSize() )

		with context :
			self.assertEqual( scale["in"]["format"].hash(), scale["out"]["format"].hash() )
			self.assertEqual( scale["in"]["channelNames"].hash(), scale["out"]["channelNames"].hash() )
			self.assertEqual( scale["in"]["dataWindow"].hash(), scale["out"]["dataWindow"].hash() )
			self.assertEqual( scale["in"]["channelData"].hash(), scale["out"]["channelData"].hash() )
		
			scale["enabled"].setValue( True )
			scale["scale"].setValue( IECore.V2f( .25, .75 ) )	
			
			self.assertEqual( scale["in"]["format"].hash(), scale["out"]["format"].hash() )
			self.assertEqual( scale["in"]["channelNames"].hash(), scale["out"]["channelNames"].hash() )
			self.assertNotEqual( scale["in"]["dataWindow"].hash(), scale["out"]["dataWindow"].hash() )
			self.assertNotEqual( scale["in"]["channelData"].hash(), scale["out"]["channelData"].hash() )

	def testHashPassThrough( self ) :
		
		read = GafferImage.ImageReader()
		read["fileName"].setValue( os.path.join( self.path, "blueWithDataWindow.100x100.exr" ) )
		scale = GafferImage.Scale()
		scale["in"].setInput( read["out"] )
		
		context = Gaffer.Context()
		context["image:channelName"] = "B"
		context["image:tileOrigin"] = IECore.V2i( GafferImage.ImagePlug.tileSize() )

		with context :
			scale["scale"].setValue( IECore.V2f( 1., 1. ) )	
			self.assertEqual( scale["in"]["format"].hash(), scale["out"]["format"].hash() )
			self.assertEqual( scale["in"]["channelNames"].hash(), scale["out"]["channelNames"].hash() )
			self.assertEqual( scale["in"]["dataWindow"].hash(), scale["out"]["dataWindow"].hash() )
			self.assertEqual( scale["in"]["channelData"].hash(), scale["out"]["channelData"].hash() )
		
			scale["scale"].setValue( IECore.V2f( .25, .75 ) )	
			formatHash = scale["out"]["format"].hash()
			dataWindowHash = scale["out"]["dataWindow"].hash()
			channelNamesHash = scale["out"]["channelNames"].hash()
			channelDataHash = scale["out"]["channelData"].hash()
			self.assertEqual( scale["in"]["format"].hash(), formatHash )
			self.assertNotEqual( scale["in"]["dataWindow"].hash(), dataWindowHash )
			self.assertNotEqual( scale["in"]["channelData"].hash(), channelDataHash )
			self.assertEqual( scale["in"]["channelNames"].hash(), channelNamesHash )
			
			scale["filter"].setValue( "Hermite" )	
			self.assertNotEqual( channelDataHash, scale["out"]["channelData"].hash() )
			self.assertEqual( dataWindowHash, scale["out"]["dataWindow"].hash() )
			self.assertEqual( formatHash, scale["out"]["format"].hash() )
			self.assertEqual( channelNamesHash, scale["out"]["channelNames"].hash() )
			formatHash = scale["out"]["format"].hash()
			dataWindowHash = scale["out"]["dataWindow"].hash()
			channelNamesHash = scale["out"]["channelNames"].hash()
			channelDataHash = scale["out"]["channelData"].hash()
			
			scale["origin"].setValue( IECore.V2f( 30., 21. ) )	
			self.assertNotEqual( channelDataHash, scale["out"]["channelData"].hash() )
			self.assertNotEqual( dataWindowHash, scale["out"]["dataWindow"].hash() )
			self.assertEqual( formatHash, scale["out"]["format"].hash() )
			self.assertEqual( channelNamesHash, scale["out"]["channelNames"].hash() )

	def testDirtyPropagation( self ) :
	
		read = GafferImage.ImageReader()
		read["fileName"].setValue( os.path.join( self.path, "blueWithDataWindow.100x100.exr" ) )
		scale = GafferImage.Scale()
		scale["in"].setInput( read["out"] )
		
		context = Gaffer.Context()
		context["image:channelName"] = "B"
		context["image:tileOrigin"] = IECore.V2i( 0 )

		with context :
			cs = GafferTest.CapturingSlot( scale.plugDirtiedSignal() )
			scale["scale"].setValue( IECore.V2f( .2, .2 ) )	
		
			dirtiedPlugs = set( [ x[0].relativeName( x[0].node() ) for x in cs ] )
			self.assertEqual( len( dirtiedPlugs ), 6 )
			self.assertTrue( "scale" in dirtiedPlugs )
			self.assertTrue( "scale.x" in dirtiedPlugs )
			self.assertTrue( "scale.y" in dirtiedPlugs )
			self.assertTrue( "out.channelData" in dirtiedPlugs )
			self.assertTrue( "out" in dirtiedPlugs )
			self.assertTrue( "out.dataWindow" in dirtiedPlugs )

		with context :
			cs = GafferTest.CapturingSlot( scale.plugDirtiedSignal() )
			scale["filter"].setValue( "Hermite" )	
		
			dirtiedPlugs = set( [ x[0].relativeName( x[0].node() ) for x in cs ] )
			self.assertEqual( len( dirtiedPlugs ), 3 )
			self.assertTrue( "filter" in dirtiedPlugs )
			self.assertTrue( "out.channelData" in dirtiedPlugs )
			self.assertTrue( "out" in dirtiedPlugs )

		with context :
			cs = GafferTest.CapturingSlot( scale.plugDirtiedSignal() )
			scale["origin"].setValue( IECore.V2f( 10., 25. ) )	
		
			dirtiedPlugs = set( [ x[0].relativeName( x[0].node() ) for x in cs ] )
			self.assertEqual( len( dirtiedPlugs ), 6 )
			self.assertTrue( "origin" in dirtiedPlugs )
			self.assertTrue( "origin.x" in dirtiedPlugs )
			self.assertTrue( "origin.y" in dirtiedPlugs )
			self.assertTrue( "out.channelData" in dirtiedPlugs )
			self.assertTrue( "out.dataWindow" in dirtiedPlugs )
			self.assertTrue( "out" in dirtiedPlugs )

