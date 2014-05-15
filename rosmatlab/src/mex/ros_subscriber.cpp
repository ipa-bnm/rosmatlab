//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#include <rosmatlab/mex.h>
#include <rosmatlab/ros.h>
#include <rosmatlab/options.h>

using namespace rosmatlab;

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
  static MexMethodMap<Subscriber> methods;

  if (!methods.initialize()) {
    methods
      .add("subscribe", &Subscriber::subscribe)
      .add("poll", &Subscriber::poll)
      .add("getTopic", &Subscriber::getTopic)
      .add("getDataType", &Subscriber::getDataType)
      .add("getMD5Sum", &Subscriber::getMD5Sum)
      .add("getNumPublishers", &Subscriber::getNumPublishers)
      .add("getConnectionHeader", &Subscriber::getConnectionHeader)
      .add("getReceiptTime", &Subscriber::getReceiptTime)
      .throwOnUnknown();
  }

  //Load the global pointer if it is already set, else set it
  	std::cout << "Try to access the pointer-variable from the workspace" << std::endl;
  	mxArray * g_vars_address;
  	g_vars_address = mexGetVariable("global", "rosmatlab_cppintrospection_gvars_pointer");
  	bool isValidPointer = g_vars_address!=0;
  	if (isValidPointer)
  		isValidPointer = mxIsDouble(g_vars_address);
  	if (!isValidPointer)
  	{
  		cpp_introspection::gvars = new cpp_introspection::G_Vars;
  		g_vars_address = mxCreateDoubleMatrix(1,1,mxREAL);
  		mxGetPr(g_vars_address)[0] = static_cast<double>((size_t) cpp_introspection::gvars);
  		mexPutVariable("global", "rosmatlab_cppintrospection_gvars_pointer", g_vars_address);
  		std::cout << "Created new global pointer at address " << cpp_introspection::gvars << std::endl;
  	} else
  	{
  		std::cout << "It's a double!" << std::endl;
  		cpp_introspection::gvars = (cpp_introspection::G_Vars *) static_cast<int>(mxGetPr(g_vars_address)[0]);
  		std::cout << "Loaded an existing pointer at address " << cpp_introspection::gvars << std::endl;
  	}

  try {
    /* Initialize ROS node */
    init();

    /* Subscriber *subscriber = */
    mexClassHelper<Subscriber>(nlhs, plhs, nrhs, prhs, methods);

  } catch(Exception &e) {
    mexErrMsgTxt(e.what());
  }
}
