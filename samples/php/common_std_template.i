namespace std {
  %template(Utf16CharVector) vector<uint16_t>;
  %template(StringVector) vector<string>;
  %template(StringVector2d) vector<vector<string> >;
  %template(OcrCharVariantVector) vector<se::smartid::OcrCharVariant>;
  %template(OcrCharVector) vector<se::smartid::OcrChar>;
  %template(MatchResultVector) vector<se::smartid::MatchResult>;
  %template(SegmentationResultVector) vector<se::smartid::SegmentationResult>;

  %template(StringCollection) map<string, string>;
  %template(StringVectorCollection) map<string, vector<string> >;
  %template(ImageFieldCollection) map<std::string, se::smartid::ImageField>;
  %template(StringFieldCollection) map<std::string, se::smartid::StringField>;
  %template(ForensicFieldCollection) map<std::string, se::smartid::ForensicField>;
  %template(QuadrangleCollection) map<std::string, se::smartid::Quadrangle>;
  %template(IntegratedFieldStateCollection) map<std::string, se::smartid::IntegratedFieldState>;
}
