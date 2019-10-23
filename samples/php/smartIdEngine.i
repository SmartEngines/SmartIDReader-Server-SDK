%include std_map.i
%include std_string.i
%include std_vector.i

%include typemaps.i
%include carrays.i
%include stdint.i

%typemap(typecheck) unsigned char* { }
%apply(char* ) { unsigned char*  data};

%typemap(throws) std::exception {
  zend_throw_exception(NULL, $1.what(), 0 TSRMLS_CC);
  return;
}

%{
  #include "smartid_common.h"
  #include "smartid_result.h"
  #include "smartid_engine.h"
%}

%feature("notabstract") SessionSettings;
%feature("notabstract") RecognitionSession;

%nodefaultctor SessionSettings;
%nodefaultctor RecognitionSession;

%array_functions(unsigned char, SWIG_UnsignedCharArray)

%newobject se::smartid::RecognitionEngine::CreateSessionSettings;
%newobject se::smartid::RecognitionEngine::SpawnSession;

%include "common_ignore.i"
%ignore se::smartid::Image::CopyToBuffer;
%ignore se::smartid::Image::CopyBase64ToBuffer;

%rename(CloneSessionSettings) se::smartid::SessionSettings::Clone;

%include "smartid_common.h"
%include "smartid_result.h"
%include "smartid_engine.h"

%include "common_std_template.i"