%module pySmartIdEngine

%include std_map.i
%include std_string.i
%include std_vector.i
%include pybuffer.i
%include carrays.i
%typemap(typecheck) unsigned char* { }
%pybuffer_mutable_string(char* out_buffer); 
%pybuffer_mutable_string(unsigned char *data); 
%pybuffer_mutable_string(unsigned char *yuv_data); 
%pybuffer_mutable_string(const unsigned char* buffer);  

%{
  #include "smartid_common.h"
  #include "smartid_result.h"
  #include "smartid_engine.h"
%}

%include "common_ignore.i"

%rename("%(lowercamelcase)s", %$isvariable) "";

%newobject se::smartid::SessionSettings::Clone;
%newobject se::smartid::RecognitionEngine::CreateSessionSettings;
%newobject se::smartid::RecognitionEngine::SpawnSession;

%include "smartid_common.h"
%include "smartid_result.h"
%include "smartid_engine.h"

%include "common_std_template.i"
