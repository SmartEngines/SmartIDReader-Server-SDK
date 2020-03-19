/**
 * Copyright (c) 2012-2020, Smart Engines Ltd
 * All rights reserved.
 */

/**
 * @file smartid_result.h
 * @brief Recognition result classes
 */

#ifndef SMARTID_ENGINE_SMARTID_RESULT_H_INCLUDED_
#define SMARTID_ENGINE_SMARTID_RESULT_H_INCLUDED_

#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4290)
#endif

#include "smartid_common.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace se { namespace smartid {

/**
 * @brief Possible character recognition result
 */
class SMARTID_DLL_EXPORT OcrCharVariant {
public:
  /**
   * @brief Default ctor
   */
  OcrCharVariant();

  /// OcrCharVariant dtor
  ~OcrCharVariant();

  /**
   * @brief Ctor from utf16 character and confidence
   * @param utf16_char - Utf16-character of a symbol
   * @param confidence - double confidence in range [0..1]
   *
   * @throw std::invalid_argument if confidence is not in range [0..1]
   */
  OcrCharVariant(uint16_t utf16_char, double confidence) throw(std::exception);

  /**
   * @brief Ctor from utf8 character and confidence
   * @param utf8_char - utf8-representation of a 2-byte symbol in std::string
   * form
   * @param confidence - double confidence in range [0..1]
   *
   * @throw std::invalid_argument if confidence is not in range [0..1] or
   *        if utf8_char is not a correct utf8 representation of 2-byte symbol
   */
  OcrCharVariant(const std::string& utf8_char,
                 double confidence) throw(std::exception);

  /// Getter for character in Utf16 form
  uint16_t GetUtf16Character() const;
  /// Getter for character in Utf8 form
  std::string GetUtf8Character() const;
  /// Variant confidence (pseudoprobability), in range [0..1]
  double GetConfidence() const;

private:
  uint16_t character_;
  double confidence_;
};

/**
 * @brief Contains all OCR information for a given character
 */
class SMARTID_DLL_EXPORT OcrChar {
public:
  /**
   * @brief Default ctor
   */
  OcrChar();

  /**
   * @brief Main ctor
   * @param ocr_char_variants - vector of char variants
   * @param is_highlighted - whether this OcrChar is highlighted as unconfident
   * @param is_corrected - whether this OcrChar was corrected by post-processing
   */
  OcrChar(const std::vector<OcrCharVariant>& ocr_char_variants,
          bool is_highlighted, bool is_corrected,
          const Rectangle& ocr_char_rect = {});

  /// OcrChar dtor
  ~OcrChar();

  /// Vector with possible recognition results for a given character
  const std::vector<OcrCharVariant>& GetOcrCharVariants() const;

  /// Whether this character is 'highlighted' (not confident) by the system
  bool IsHighlighted() const;
  /// Whether this character was changed by context correction (postprocessing)
  bool IsCorrected() const;

  /**
   * @brief Returns the most confident character as 16-bit utf16 character
   *
   * @throws std::out_of_range if variants are empty
   */
  uint16_t GetUtf16Character() const throw(std::exception);

  /**
   * @brief Returns the most confident character as utf8 representation of
   * 16-bit character
   *
   * @throws std::out_of_range if variants are empty
   */
  std::string GetUtf8Character() const throw(std::exception);

  /**
   * @brief Returns the rect position of char on field's image
   *
   */

  const Rectangle& GetRectangle() const;

private:
  std::vector<OcrCharVariant> ocr_char_variants_;
  bool is_highlighted_;
  bool is_corrected_;
  Rectangle rect_;
};

/**
 * @brief Contains additional OCR information for the whole string
 */
class SMARTID_DLL_EXPORT OcrString {
public:
  /// Default ctor
  OcrString();
  /// Ctor from vector of OcrChars
  OcrString(const std::vector<OcrChar>& ocr_chars);
  /**
   * @brief OcrString ctor from plain utf8 string
   */
  OcrString(const std::string& utf8_string);
  /// OcrString dtor
  ~OcrString();

  /// Vector with OCR information for each character
  const std::vector<OcrChar>& GetOcrChars() const;

  /// Returns the most-confident string representation
  std::string GetUtf8String() const;

  /// Returns the most-confident string representation
  std::vector<uint16_t> GetUtf16String() const;

private:
  std::vector<OcrChar> ocr_chars_; ///< Vector with OCR information for each character
};

/**
 * @brief Class represents implementation of SmartID document Field for string
 * fields
 */
class SMARTID_DLL_EXPORT StringField {
public:
  /**
   * @brief Default constructor
   */
  StringField();

  /**
   * @brief StringField main ctor
   * @param name - name of the field
   * @param value - OcrString-representation of the field value
   * @param is_accepted - whether the system is confident in the field's value
   * @param confidence - system's confidence level in fields' value in range [0..1\
   * @param attributes - additional field information
   *
   * @throws std::invalid_argument if confidence value is not in range [0..1]
   */
  StringField(const std::string& name, const OcrString& value,
              bool is_accepted, double confidence,
              const std::map<std::string, std::string>& attributes = {}) throw(std::exception);

  /**
   * @brief StringField ctor from utf8-string value
   * @param name - name of the field
   * @param value - utf8-string representation of the field value
   * @param is_accepted - whether the system is confident in the field's value
   * @param confidence - system's confidence level in fields' value in range [0..1]
   * @param attributes - additional field information
   *
   * @throws std::invalid_argument if confidence value is not in range [0..1] or
   *         if failed to decode utf8-string 'value'
   */
  StringField(const std::string& name, const std::string& value,
              bool is_accepted, double confidence,
              const std::map<std::string, std::string>& attributes = {}) throw(std::exception);

  /**
   * @brief Destructor
   */
  ~StringField();

  /// Getter for string field name
  const std::string& GetName() const;
  /// Getter for string field value (OcrString representation)
  const OcrString& GetValue() const;
  /// Getter for string field value (Utf8-string representation)
  std::string GetUtf8Value() const;
  /// Whether the system is confident in field recognition result
  bool IsAccepted() const;
  /// The system's confidence level in field recognition result (in range
  /// [0..1])
  double GetConfidence() const;

  /// Returns a vector of attribute names
  std::vector<std::string> GetAttributeNames() const;

  /// Getter for attributes map
  const std::map<std::string, std::string> &GetAttributes() const;

  /**
   * @brief Check if attribute with given name is present
   * @param attribute_name attribute name to check presence of
   * @return true if attribute with given name is present
   */
  bool HasAttribute(const std::string &attribute_name) const;

  /**
   * @brief Get attribute value by its name
   * @param attribute_name key attribute name
   * @return attribute value by its name
   */
  const std::string &GetAttribute(const std::string &attribute_name) const
      throw(std::exception);

private:
  std::string name_; ///< Field name
  OcrString value_;  ///< Fields' OcrString value

  /// Specifies whether the system is confident in field recognition result
  bool is_accepted_;
  /// Specifies the system's confidence level in field recognition result
  double confidence_;

  std::map<std::string, std::string> attributes_; ///< Field attributes
};

/**
 * @brief Class represents implementation of SmartIDField for list of images
 */
class SMARTID_DLL_EXPORT ImageField {
public:
  /**
   * @brief ImageField Default ctor
   */
  ImageField();

  /**
   * @brief ImageField main ctor
   * @param name - name of the field
   * @param value - image (the field result)
   * @param is_accepted - whether the system is confident in the field's value
   * @param confidence - system's confidence level in fields' value in range
   * [0..1]
   *
   * @throws std::invalid_argument if confidence value is not in range [0..1] or
   *         if failed to decode utf8-string 'value'
   */
  ImageField(const std::string& name, const Image& value, bool is_accepted,
             double confidence, const std::map<std::string, std::string>& attributes = {}) throw(std::exception);

  /// Default dtor
  ~ImageField();

  /// Getter for image field name
  const std::string& GetName() const;
  /// Getter for image field result
  const Image& GetValue() const;
  /// Whether the system is confidence in field result
  bool IsAccepted() const;
  /// The system's confidence level in field result (in range [0..1])
  double GetConfidence() const;

  /// Returns a vector of attribute names
  std::vector<std::string> GetAttributeNames() const;

  /// Getter for attributes map
  const std::map<std::string, std::string> &GetAttributes() const;

  /**
   * @brief Check if attribute with given name is present
   * @param attribute_name attribute name to check presence of
   * @return true if attribute with given name is present
   */
  bool HasAttribute(const std::string &attribute_name) const;

  /**
   * @brief Get attribute value by its name
   * @param attribute_name key attribute name
   * @return attribute value by its name
   */
  const std::string &GetAttribute(const std::string &attribute_name) const
    throw(std::exception);

private:
  std::string name_; ///< Image field name
  Image value_;      ///< Image field value (internal image storage)

  bool is_accepted_;  ///< Specifies whether the system is confident in result
  double confidence_; ///< Specifies the system's confidence level in result

  std::map<std::string, std::string> attributes_; ///< Field attributes

};

/**
 * @brief Class represents SmartID match result
 */
class SMARTID_DLL_EXPORT MatchResult {
public:
  /**
   * @brief Default ctor
   */
  MatchResult();

  /**
   * @brief MatchResult main ctor
   * @param tpl_type - template type for this match result
   * @param quadrangle - quadrangle of a template on image
   * @param accepted - acceptance for visualization
   */
  MatchResult(const std::string& tpl_type,
              const Quadrangle& quadrangle,
              bool accepted = false,
              double confidence = 0.0,
              int standard_width = 0,
              int standard_height = 0);

  /**
   * @brief Destructor
   */
  ~MatchResult();

  /// Getter for document type string
  const std::string& GetTemplateType() const;
  /// Getter for document quadrangle
  const Quadrangle& GetQuadrangle() const;
  /// Getter for standard template width in pixels
  int GetStandardWidth() const;
  /// Getter for standard template height in pixels
  int GetStandardHeight() const;
  /// Getter for acceptance field
  bool GetAccepted() const;
  /// Getter for confidence field
  double GetConfidence() const;

private:
  std::string template_type_; ///< Template type for this match result
  Quadrangle quadrangle_; ///< Quadrangle for this template
  int standard_width_;    ///< Standard width for this template type
  int standard_height_;   ///< Standard height for this template type
  bool accepted_; ///< Whether this result is ready to be visualized
  double confidence_; ///< System's confidence level in match result
};

/**
 * @brief Class represents implementation of SmartID forensic field for document
 * validity checks
 */
class SMARTID_DLL_EXPORT ForensicField {
public:
  /**
   * @brief Default constructor
   */
  ForensicField();

  /**
   * @brief ForensicField main ctor
   * @param name - name of the field
   * @param value - string-representation of the field value
   * @param is_accepted - whether the system is confident in the field's value
   * @param confidence - system's confidence level in fields' value in range [0..1\
   * @param attributes - additional field information
   *
   * @throws std::invalid_argument if confidence value is not in range [0..1]
   */
  ForensicField(const std::string& name,
                const std::string& value,
                bool is_accepted,
                double confidence,
                const std::map<std::string, std::string>& attributes = {}) throw(std::exception);
  /**
   * @brief Destructor
   */
  ~ForensicField();

  /// Getter for string field name
  const std::string& GetName() const;
  /// Getter for string field value (string representation)
  const std::string& GetValue() const;
  /// Whether the system is confident in field recognition result
  bool IsAccepted() const;
  /// The system's confidence level in field recognition result (in range
  /// [0..1])
  double GetConfidence() const;

  /// Returns a vector of attribute names
  std::vector<std::string> GetAttributeNames() const;

  /// Getter for attributes map
  const std::map<std::string, std::string> &GetAttributes() const;

  /**
   * @brief Check if attribute with given name is present
   * @param attribute_name attribute name to check presence of
   * @return true if attribute with given name is present
   */
  bool HasAttribute(const std::string &attribute_name) const;

  /**
   * @brief Get attribute value by its name
   * @param attribute_name key attribute name
   * @return attribute value by its name
   */
  const std::string &GetAttribute(const std::string &attribute_name) const
      throw(std::exception);

private:
  std::string name_; ///< Field name
  std::string value_;  ///< Fields' value

  /// Specifies whether the system is confident in field recognition result
  bool is_accepted_;
  /// Specifies the system's confidence level in field recognition result
  double confidence_;

  std::map<std::string, std::string> attributes_; ///< Field attributes
};

/**
 * @brief Class represents SmartID segmentation result containing
 * found raw fields location information
 */
class SMARTID_DLL_EXPORT SegmentationResult {
public:
  /// Default constructor
  SegmentationResult();

  /// Main constructor
  SegmentationResult(
      const std::map<std::string, Quadrangle>& raw_fields_quadrangles,
      const std::map<std::string, Quadrangle>& raw_fields_template_quadrangles,
      bool accepted = false);

  /**
   * @brief Destructor
   */
  ~SegmentationResult();

  /// Getter for raw fields names which are keys for RawFieldQuadrangles map
  std::vector<std::string> GetRawFieldsNames() const;

  /// Checks if there is a raw field quadrangle with given raw field name
  bool HasRawFieldQuadrangle(const std::string &raw_field_name) const;

  /**
   * @brief Get raw field quadrangle for raw field name
   * @param raw_field_name Raw field name
   * @return Raw field quadrangle for raw field name
   * @throws std::invalid_argument if raw_field_name is not present in raw field quadrangles
   */
  const Quadrangle& GetRawFieldQuadrangle(const std::string &raw_field_name) const throw (std::exception);

  /// Getter for raw field quadrangles (raw field name -> quadrangle]
  const std::map<std::string, Quadrangle>& GetRawFieldQuadrangles() const;

  /**
   * @brief Get raw field quadrangle for raw field name in template coordinates
   * @param raw_field_name Raw field name
   * @return Raw field quadrangle for raw field name in template coordinates
   * @throws std::invalid_argument if raw_field_name is not present in raw field quadrangles
   */
  const Quadrangle& GetRawFieldTemplateQuadrangle(const std::string &raw_field_name) const throw (std::exception);

  /// Getter for raw field quadrangles in template coordinates (raw field name -> quadrangle]
  const std::map<std::string, Quadrangle>& GetRawFieldTemplateQuadrangles() const;

  /// Getter for accepted field
  bool GetAccepted() const;

private:
  /// [raw field name, quadrangle]
  std::map<std::string, Quadrangle> raw_field_quadrangles_;
  /// [raw field name, quadrangle in template coords]
  std::map<std::string, Quadrangle> raw_field_template_quadrangles_;
  /// Whether this result is ready to be visualized
  bool accepted_;
};

/**
 * @brief Class represents SmartID recognition result
 */
class SMARTID_DLL_EXPORT RecognitionResult {
public:
  /**
   * @brief Default ctor
   */
  RecognitionResult();

  /**
   * @brief RecognitionResult main ctor
   */
  RecognitionResult(const std::map<std::string, StringField>& string_fields,
                    const std::map<std::string, ImageField>& image_fields,
                    const std::map<std::string, ForensicField>& forensic_fields,
                    const std::map<std::string, StringField>& raw_string_fields,
                    const std::map<std::string, ImageField>& raw_image_fields,
                    const std::string& document_type,
                    const std::vector<MatchResult>& match_results,
                    const std::vector<SegmentationResult>& segmentation_results,
                    bool is_terminal);

  /// RecognitionResult dtor
  ~RecognitionResult();

  //////////////////// String Fields ////////////////////

  /// Returns a vector of unique string field names
  std::vector<std::string> GetStringFieldNames() const;
  /// Checks if there is a string field with given name
  bool HasStringField(const std::string& name) const;

  /**
   * @brief Gets string field by name
   * @param name - name of a string field
   *
   * @throws std::invalid_argument if there is no such field
   */
  const StringField& GetStringField(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for string fields map
   * @return constref for string fields map
   */
  const std::map<std::string, StringField>& GetStringFields() const;

  /**
   * @brief Getter for (mutable) string fields map
   * @return ref for string fields map
   */
  std::map<std::string, StringField>& GetStringFields();

  /**
   * @brief Setter for string fields map
   * @param string_fields - string fields map
   */
  void SetStringFields(const std::map<std::string, StringField>& string_fields);

  //////////////////// Image Fields ////////////////////

  /// Returns a vector of unique image field names
  std::vector<std::string> GetImageFieldNames() const;
  /// Checks if there is a image field with given name
  bool HasImageField(const std::string& name) const;

  /**
   * @brief Gets image field by name
   * @param name - name of an image field
   *
   * @throws std::invalid_argument if there is no such field
   */
  const ImageField& GetImageField(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for image fields map
   * @return constref for image fields map
   */
  const std::map<std::string, ImageField>& GetImageFields() const;

  /**
   * @brief Getter for (mutable) image fields map
   * @return ref for image fields map
   */
  std::map<std::string, ImageField>& GetImageFields();

  /**
   * @brief Setter for image fields map
   * @param image_fields - image fields map
   */
  void SetImageFields(const std::map<std::string, ImageField>& image_fields);

  //////////////////// Forensic Fields ////////////////////

  /// Returns a vector of unique forensic field names
  std::vector<std::string> GetForensicFieldNames() const;
  /// Checks if there is a forensic field with given name
  bool HasForensicField(const std::string& name) const;

  /**
   * @brief Gets forensic field by name
   * @param name - name of a forensic field
   *
   * @throws std::invalid_argument if there is no such field
   */
  const ForensicField& GetForensicField(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for forensic fields map
   * @return constref for forensic fields map
   */
  const std::map<std::string, ForensicField>& GetForensicFields() const;

  /**
   * @brief Getter for (mutable) forensic fields map
   * @return ref for forensic fields map
   */
  std::map<std::string, ForensicField>& GetForensicFields();

  /**
   * @brief Setter for forensic fields map
   * @param forensic_fields - string fields map
   */
  void SetForensicFields(
      const std::map<std::string, ForensicField>& forensic_fields);

  ///// Raw (only current frame, no postprocessing) String Fields /////

  /// Returns a vector of unique raw string field names
  std::vector<std::string> GetRawStringFieldNames() const;
  /// Checks if there is a raw string field with given name
  bool HasRawStringField(const std::string& name) const;

  /**
   * @brief Gets raw string field by name
   * @param name - name of a raw string field
   *
   * @throws std::invalid_argument if there is no such field
   */
  const StringField& GetRawStringField(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for raw string fields map
   * @return constref for raw string fields map
   */
  const std::map<std::string, StringField>& GetRawStringFields() const;

  /**
   * @brief Getter for (mutable) raw string fields map
   * @return ref for raw string fields map
   */
  std::map<std::string, StringField>& GetRawStringFields();

  /**
   * @brief Setter for raw string fields map
   * @param raw_string_fields - raw string fields map
   */
  void SetRawStringFields(const std::map<std::string, StringField>& raw_string_fields);

  ///// Raw (only current frame, no postprocessing) Image Fields /////

  /// Returns a vector of unique raw image field names
  std::vector<std::string> GetRawImageFieldNames() const;
  /// Checks if there is a raw image field with given name
  bool HasRawImageField(const std::string& name) const;

  /**
   * @brief Gets raw image field by name
   * @param name - raw name of an image field
   *
   * @throws std::invalid_argument if there is no such field
   */
  const ImageField& GetRawImageField(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for raw image fields map
   * @return constref for raw image fields map
   */
  const std::map<std::string, ImageField>& GetRawImageFields() const;

  /**
   * @brief Getter for (mutable) raw image fields map
   * @return ref for raw image fields map
   */
  std::map<std::string, ImageField>& GetRawImageFields();

  /**
   * @brief Setter for raw image fields map
   * @param raw_image_fields - raw image fields map
   */
  void SetRawImageFields(const std::map<std::string, ImageField>& raw_image_fields);

  //////////////////// Document Type ////////////////////

  /// Getter for document type name. Empty string means empty result (no
  /// document match happened yet)
  const std::string& GetDocumentType() const;

  /// Setter for document type name
  void SetDocumentType(const std::string& doctype);

  //////////////////// Match Results ////////////////////

  /// Getter for match results - contains the most 'fresh' template quadrangles
  /// information available
  const std::vector<MatchResult>& GetMatchResults() const;
  /// Setter for match results
  void SetMatchResults(const std::vector<MatchResult>& match_results);

  //////////////////// Segmentation Results ////////////////////

  /// Getter for segmentation results - contains the most 'fresh' raw fields
  /// and fields location information available
  const std::vector<SegmentationResult>& GetSegmentationResults() const;
  /// Setter for segmentation results
  void SetSegmentationResults(const std::vector<SegmentationResult>& segmentation_results);

  //////////////////// Is Terminal ////////////////////

  /**
   * @brief Whether the systems regards that result as 'final'.
   *        Could be (optionally) used to stop the recognition session.
   */
  bool IsTerminal() const;
  /// Setter for IsTerminal flag
  void SetIsTerminal(bool is_terminal);

  //////////////////// Jpeg compression ////////////////////
  /// Getter for source application or device, that performed jpeg compression.
  /// Empty string means that document is not jpeg-compressed or has unknown source
  const std::string& GetJpegCompression() const;

  /// Setter for jpeg compression
  void SetJpegCompression(const std::string& jpeg_compression);

private:
  std::map<std::string, StringField> string_fields_;
  std::map<std::string, ImageField> image_fields_;
  std::map<std::string, ForensicField> forensic_fields_;

  std::map<std::string, StringField> raw_string_fields_;
  std::map<std::string, ImageField> raw_image_fields_;
  std::string document_type_;
  std::vector<MatchResult> match_results_;
  std::vector<SegmentationResult> segmentation_results_;
  bool is_terminal_;
  std::string jpeg_compression_;
};

/**
 * @brief Feedback data that is returned by the ResultReporterInterface's
 * FeedbackReceived method, containing useful user-oriented information
 * such as additional visualization, advisory information etc
 */
class SMARTID_DLL_EXPORT ProcessingFeedback {
public:
  /// Default constructor
  ProcessingFeedback();

  /// Main constructor
  ProcessingFeedback(const std::map<std::string, Quadrangle> &quadrangles);

  /// Destructor
  ~ProcessingFeedback();

  /**
   * @brief Getter for arbitrary quadrangles feedback data
   * @return map with quadrangles feedback data
   */
  const std::map<std::string, Quadrangle>& GetQuadrangles() const;

private:
  std::map<std::string, Quadrangle> quadrangles_; ///< quadrangle data
};

/**
 * @brief Callback interface to obtain recognition results. Must be implemented
 *        to get the results as they appear during the stream processing
 */
class SMARTID_DLL_EXPORT ResultReporterInterface {
public:

  /**
   * @brief  Callback tells that last snapshot is not going to be
   *         processed/recognized. Optional
   */
  virtual void SnapshotRejected();

  /**
   * @brief FeedbackReceived
   * @param processing_feedback processing feedback data returned by the core
   */
  virtual void FeedbackReceived(const ProcessingFeedback& processing_feedback);

  /**
   * @brief  Callback tells that last snapshot has valid document and
             contains document match result. Optional
   * @param  match_results   Document match result - vector of found templates
   */
  virtual void DocumentMatched(const std::vector<MatchResult>& match_results);

  /**
   * @brief  Callback tells that last snapshot was segmented into raw fields
   *         for each match result. Optional.
   * @param segmentation_results Segmentation results for each corresponding MatchResult
   */
  virtual void DocumentSegmented(const std::vector<SegmentationResult>& segmentation_results);

  /**
   * @brief  Callback tells that last snapshot was processed
   *         successfully and returns current result. Required
   * @param  recog_result       Current recognition result
   */
  virtual void SnapshotProcessed(const RecognitionResult& recog_result) = 0;

  /**
   * @brief Internal callback to stop the session (determined by internal timer)
   */
  virtual void SessionEnded();

  /**
   * @brief  Destructor
   */
  virtual ~ResultReporterInterface();
};

/**
 * @brief IntegratedFieldState class - integrated field terminality state
 */
class SMARTID_DLL_EXPORT IntegratedFieldState {
public:
  /**
   * @brief Default ctor
   */
  explicit IntegratedFieldState(bool is_terminal = false);

  /**
   * @brief Whether the systems regards that result for the field as 'final'.
   */
  bool IsTerminal() const;
  /// Setter for IsTerminal flag
  void SetIsTerminal(bool is_terminal);

private:
  bool is_terminal_;
};

/**
 * @brief SessionState class - optional recognition session information
 */
class SMARTID_DLL_EXPORT SessionState {
public:
  virtual ~SessionState();

  /// Returns a vector of unique integrated field state names
  std::vector<std::string> GetIntegratedFieldStateNames() const;
  /// Checks if there is an integrated field state with given name
  bool HasIntegratedFieldState(const std::string& name) const;

  /**
   * @brief Gets integrated field state by name
   * @param name - name of an integrated field state
   *
   * @throws std::invalid_argument if there is no such field state
   */
  const IntegratedFieldState& GetStringFieldState(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Getter for integrated field states map
   * @return constref for integrated field states map
   */
  const std::map<std::string, IntegratedFieldState>& GetIntegratedFieldStates() const;

  /**
   * @brief Getter for (mutable) integrated field states map
   * @return ref for integrated field states map
   */
  std::map<std::string, IntegratedFieldState>& GetIntegratedFieldStates();

  /**
   * @brief Setter for integrated field states map
   * @param integrated_field_states - integrated field states map
   */
  void SetIntegratedFieldStates(const std::map<std::string, IntegratedFieldState>& integrated_field_states);

  int GetSnapshotsProcessed() const;

protected:
  std::map<std::string, IntegratedFieldState> integrated_field_states_;
  int snapshots_processed_;

  /// Disabled default constructor - use ... instead
  SessionState(int snapshots_processed);
};

} } // namespace se::smartid

#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // SMARTID_ENGINE_SMARTID_RESULT_H_INCLUDED
