<?php
  require(realpath(dirname(__FILE__)).DIRECTORY_SEPARATOR."../../bindings/php/phpSmartIdEngine.php");

function OutputRecognitionResult($recog_result) {
  printf("Document type: %s\n", $recog_result->GetDocumentType());
  printf("Match results:\n");
  $match_results = $recog_result->GetMatchResults();
  for ($i = 0; $i < $match_results->size(); $i++) {
    $match_result_quad = $match_results->get($i)->GetQuadrangle();
    printf("    Template Type = %s\n", $match_results->get($i)->GetTemplateType());
    printf("    Zone = { (%f, %f), (%f, %f), (%f, %f), (%f, %f) }\n", 
        $match_result_quad->GetPoint(0)->x, $match_result_quad->GetPoint(0)->y,
        $match_result_quad->GetPoint(1)->x, $match_result_quad->GetPoint(1)->y,
        $match_result_quad->GetPoint(2)->x, $match_result_quad->GetPoint(2)->y,
        $match_result_quad->GetPoint(3)->x, $match_result_quad->GetPoint(3)->y);
  }

  printf("Segmentation results:\n");
  $segmentation_results = $recog_result->GetSegmentationResults();
  for ($i = 0; $i < $segmentation_results->size(); $i++) {
    $raw_fields_names = $segmentation_results->get($i)->GetRawFieldsNames();
    for ($j = 0; $j < $raw_fields_names->size(); $j++) {
      printf("    Raw field %s\n", $raw_fields_names->get($j));
    } 
  }

  printf("String fields:\n");
  $string_field_names = $recog_result->GetStringFieldNames();
  for ($i = 0; $i < $string_field_names->size(); $i++) {
    $field = $recog_result->GetStringField($string_field_names->get($i));
    $is_accepted = $field->IsAccepted() ? " [+] " : " [-] ";
    printf("    %s\t%s\t%s\n", 
        $field->GetName(), $is_accepted, $field->GetUtf8Value());
  }

  printf("Image fields:\n");
  $image_field_names = $recog_result->GetImageFieldNames();
  for ($i = 0; $i < $image_field_names->size(); $i++) {
    $field = $recog_result->GetImageField($image_field_names->get($i));
    $is_accepted = $field->IsAccepted() ? " [+] " : " [-] ";
    printf("    %s\t%s W: %d H: %d\n", 
        $field->GetName(), $is_accepted,
        $field->GetValue()->getWidth(), $field->GetValue()->getHeight());
  }

  printf("Result terminal: %s\n", $recog_result->IsTerminal() ? " [+] " : " [-] ");
}

function main($argc, $argv) {
  if ($argc != 4) {
    printf("Usage: %s %s <path-to-image-file> <path-to-config-bundle> <document-type>\n", PHP_BINARY, $argv[0]);
    printf("Example: %s %s ../testdata/image.png ../data-zip/bundle_mrz.zip mrz.*\n", PHP_BINARY, $argv[0]);
    exit(-1);
  }

  $image_path = $argv[1];
  $config_path = $argv[2];
  $document_types = $argv[3];

  printf("SmartIDReader version: %s\n", RecognitionEngine::GetVersion());
  printf("image_path = %s\n", $image_path);
  printf("config_path = %s\n", $config_path);
  printf("document_types = %s\n", $document_types);

  try {
    $engine = new RecognitionEngine($config_path);

    $session_settings = $engine->CreateSessionSettings();
    // specify a concrete document type or wildcard mask
    $session_settings->AddEnabledDocumentTypes($document_types);

    //  load image

    $image = new Image();

      //  This works only in trial and full versions

        // $image = new Image($image_path);

    # get file as binary buffer

    $handle = fopen($image_path, "rb");
    $size = filesize($image_path);
    $contents = fread($handle, $size);
    fclose($handle);

    # get file as Base64 string
    
    $handleForBase64 = fopen(realpath(dirname(__FILE__)).DIRECTORY_SEPARATOR."base64.txt", "r");
    $sizeForBase64 = filesize($image_path);
    $base64string = fgets($handleForBase64);
    fclose($handleForBase64);

    # You can take base64 buffer directly from Image object

    // $base64stringInternal = $image->GetBase64String();

    $session = $engine->SpawnSession($session_settings, $optional_reporter); 

    // Uses engine's internal image loading, supports format: png, jpg, jpeg, tif
    $resultImage = $session->ProcessImage($image);
    $session->Reset();

    $resultFile = $session->ProcessImageFile($image_path);
    $session->Reset();
    
    $resultData = $session->ProcessImageData($contents, $size);
    $session->Reset();
    
    $resultBase64 = $session->ProcessImageDataBase64($base64string);
    $session->Reset();
    
    // $resultBase64Internal = $session->ProcessImageDataBase64($base64stringInternal);

    OutputRecognitionResult($resultImage);
    OutputRecognitionResult($resultFile);
    OutputRecognitionResult($resultData);
    OutputRecognitionResult($resultBase64);
    // OutputRecognitionResult($resultBase64Internal);

  } catch (Exception $e) {
    printf("Exception caught: %s\n", $e->getMessage());
    exit(-2);
  }
}

main($argc, $argv);

?>