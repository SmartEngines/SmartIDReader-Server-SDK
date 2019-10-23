g++ smartid_sample.cpp -O2 -std=c++11 -I ../../include -L ../../bin -l mockSmartIdEngine -o smartid_sample -Wl,-rpath,../../bin

image="../../testdata/passport_rf_2.jpg"
config=$(ls ../../data-zip/*.zip) # assuming one file
document_types="rus.passport.national"
  
./smartid_sample "$image" "$config" "$document_types"
