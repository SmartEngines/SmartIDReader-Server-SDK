#include <cstring>
#include <memory>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#include <windows.h>
#endif // _MSC_VER

#include <smartIdEngine/smartid_engine.h>

// Implementing callback interface to get the results as they appear during stream processing
class OptionalResultReporter : public se::smartid::ResultReporterInterface {
public:
  // Snapshot has been rejected due to low quality or other reasons
  virtual void SnapshotRejected() override {
    printf("[Optional callback called]: Snapshot rejected\n");
  }

  //
  virtual void FeedbackReceived(const se::smartid::ProcessingFeedback& processing_feedback) override {
    printf("[Optional callback called]: Feedback received\n");
  }
  
  // Snapshot contains a valid document, use this to obtain various zones quadrangles
  // including a whole document quadrangle
  virtual void DocumentMatched(
      const std::vector<se::smartid::MatchResult>& match_results) override {
    printf("[Optional callback called]: Document matched\n");
  }

  // Snapshot contains a valid document, use this to obtain various zones quadrangles
  // including a whole document quadrangle
  virtual void DocumentSegmented(
      const std::vector<se::smartid::SegmentationResult>& segmentation_results) override {
    printf("[Optional callback called]: Document segmented\n");
  }
  
  // Snapshot has been processed successfully, access current OCR result
  virtual void SnapshotProcessed(const se::smartid::RecognitionResult& recog_result) override {
    printf("[Optional callback called]: Snapshot processed\n");
  }

  virtual void SessionEnded() override {
    printf("[Optional callback called]: Session ended\n");
  }
};

void OutputRecognitionResult(const se::smartid::RecognitionResult& recog_result) {
  printf("Document type: %s\n", recog_result.GetDocumentType().c_str());
  
  const std::vector<se::smartid::MatchResult> match_results = recog_result.GetMatchResults();
  for (size_t i = 0; i < match_results.size(); i++) {
    printf("Match result %zu:\n", i);
    printf("    Zone = { (%lf, %lf), (%lf, %lf), (%lf, %lf), (%lf, %lf) }\n",
      match_results[i].GetQuadrangle()[0].x,
      match_results[i].GetQuadrangle()[0].y,
      match_results[i].GetQuadrangle()[1].x,
      match_results[i].GetQuadrangle()[1].y,
      match_results[i].GetQuadrangle()[2].x,
      match_results[i].GetQuadrangle()[2].y,
      match_results[i].GetQuadrangle()[3].x,
      match_results[i].GetQuadrangle()[3].y);
  }

  printf("String fields:\n");
  const auto& string_field_names = recog_result.GetStringFieldNames();
  for (const auto& field_name : string_field_names) {
    const se::smartid::StringField& field =
        recog_result.GetStringField(field_name);

    std::string is_accepted = field.IsAccepted() ? " [+] " : " [-] ";
    printf("    %-25s%s (%4.3lf) %s\n", field.GetName().c_str(),
           is_accepted.c_str(), field.GetConfidence(),
           field.GetUtf8Value().c_str());
  }

  printf("Image fields:\n");
  const auto& image_field_names = recog_result.GetImageFieldNames();
  for (const auto& field_name : image_field_names) {
    const se::smartid::ImageField& field =
        recog_result.GetImageField(field_name);
    std::string is_accepted = field.IsAccepted() ? " [+] " : " [-] ";
    printf("    %-25s%s (%4.3lf) W: %d H: %d\n", field.GetName().c_str(),
           is_accepted.c_str(), field.GetConfidence(), field.GetValue().width,
           field.GetValue().height);
  }

  printf("Result terminal:             %s\n", (recog_result.IsTerminal() == 1) ? " [+] " : " [-] ");
}

int main(int argc, char **argv) {
  
#ifdef _MSC_VER
  SetConsoleOutputCP(65001);
#endif // _MSC_VER

  if (argc != 3 && argc != 4) {
    printf(
        "Version %s. Usage: %s <image_path> <config_path> [document_types]\n",
        se::smartid::RecognitionEngine::GetVersion().c_str(), argv[0]);
    return -1;
  }

  const std::string image_path = argv[1];
  const std::string config_path = argv[2];
  const std::string document_types = (argc >= 4 ? argv[3] : "rus.passport.national");

  printf("SmartIDReader version %s\n",
         se::smartid::RecognitionEngine::GetVersion().c_str());
  printf("image_path = %s\n", image_path.c_str());
  printf("config_path = %s\n", config_path.c_str());
  printf("document_types = %s\n", document_types.c_str());
  printf("\n");

  OptionalResultReporter optional_reporter; // Optional result reporter class instance

  try {
    se::smartid::RecognitionEngine engine(config_path);

    std::unique_ptr<se::smartid::SessionSettings> settings(
          engine.CreateSessionSettings());

    // specify a concrete document type or a wildcard mask
    settings->AddEnabledDocumentTypes(document_types);


    std::unique_ptr<se::smartid::RecognitionSession> session(
        engine.SpawnSession(*settings, &optional_reporter));

    se::smartid::RecognitionResult result = session->ProcessImageFile(image_path);
    OutputRecognitionResult(result);

  } catch (const std::exception& e) {
    printf("Exception thrown: %s\n",e.what());
    return -1;
  }
  
  return 0;
}
