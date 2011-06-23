##########################################################################
#  
#  Copyright (c) 2011, John Haddon. All rights reserved.
#  Copyright (c) 2011, Image Engine Design Inc. All rights reserved.
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
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  
##########################################################################

import unittest

import IECore

import Gaffer
import GafferUI
import GafferTest

class GraphEditorTest( unittest.TestCase ) :

	def testCreateWithExistingGraph( self ) :
	
		s = Gaffer.ScriptNode()
		
		s["add1"] = GafferTest.AddNode()
		s["add2"] = GafferTest.AddNode()
		
		s["add1"]["op1"].setInput( s["add2"]["sum"] )
		
		g = GafferUI.GraphEditor( s )
		
		self.assertEqual( g.graphGadget().getFilter(), None )
	
	def testGraphGadgetAccess( self ) :
	
		s = Gaffer.ScriptNode()
		ge = GafferUI.GraphEditor( s )
		
		g = ge.graphGadget()
		
		self.failUnless( isinstance( g, GafferUI.GraphGadget ) )
	
	def testRemovedNodesDontHaveGadgets( self ) :
	
		s = Gaffer.ScriptNode()
		g = GafferUI.GraphGadget( s )
		
		n = GafferTest.AddNode()
		s["add1"] = n
		
		self.failUnless( g.nodeGadget( n ) is not None )
		
		s.deleteNodes( Gaffer.StandardSet( [ n ] ) )

		self.failUnless( g.nodeGadget( n ) is None )
		
if __name__ == "__main__":
	unittest.main()
	
