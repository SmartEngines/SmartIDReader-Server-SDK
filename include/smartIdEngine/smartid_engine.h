/**
 * Copyright (c) 2012-2017, Smart Engines Ltd
 * All rights reserved.
*/

/**
 * @file smartid_engine.h
 * @brief Main processing classes
 */

#ifndef SMARTID_ENGINE_SMARTID_ENGINE_H_INCLUDED_
#define SMARTID_ENGINE_SMARTID_ENGINE_H_INCLUDED_

#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4290)
#endif

#include <string>
#include <vector>

#include "smartid_common.h"
#include "smartid_result.h"

/**
 * @mainpage Overview
 * The Smart ID Reader Library allows to recognize various ID documents
 * on images or video data obtained either from cameras or from scanners.
 *
 * This file contains a brief description of classes and members of the Library.
 * Sample usage is shown in the @c smartid_sample.cpp.
 *
 * Feel free to send any questions about the Library on
 * support@smartengines.biz.
 */

namespace se { namespace smartid {

/**
 * @brief The SessionSettings class - runtime parameters of the recognition
 * session
 */
class SMARTID_DLL_EXPORT SessionSettings {
public:
  /// SessionSettings dtor
  virtual ~SessionSettings();

  /**
   * @brief Clones session settings and creates a new object on heap
   * @return new allocated object which is a copy of this
   */
  virtual SessionSettings * Clone() const = 0;

  /**
   * @brief Get enabled document types with which recognition session will be created
   * @return a vector of enabled document types (exact types without wildcards)
   */
  const std::vector<std::string>& GetEnabledDocumentTypes() const;

  /**
   * @brief Add enabled document types conforming to GetSupportedDocumentTypes().
   *        Both exact string type names or wildcard expression can be used, for example:
   *        "rus.passport.national", "rus.*", "*.passport.*", "*"
   * @param doctype_mask Document type name or wildcard expression
   */
  void AddEnabledDocumentTypes(const std::string &doctype_mask);

  /**
   * @brief Remove enabled document types conforming to GetEnabledDocumentTypes().
   *        Both exact string type names or wildcard expression can be used, for example:
   *        "rus.passport.national", "rus.*", "*.passport.*", "*"
   * @param doctype_mask Document type name or wildcard expression
   */
  void RemoveEnabledDocumentTypes(const std::string &doctype_mask);

  /**
   * @brief Set enabled document types. Clears all enabled types and then calls
   *        AddEnabledDocumentTypes() for each document type in the document_types
   * @param document_types a vector of enabled document types
   */
  void SetEnabledDocumentTypes(const std::vector<std::string>& document_types);

  /**
   * @brief Gets all supported document types for each engine of configured bundle.
   *        Recognition session can only be spawned with the set of document
   *        types corresponding to some single engine.
   * @return [engine][i_doctype_string] two dimensional vector const ref
   */
  const std::vector<std::vector<std::string> >& GetSupportedDocumentTypes() const;

  /**
   * @brief Get full map of additional session settings
   * @return constref map of additional options
   *
   * @details Option name is a string consisting of two components:
   *          &lt;INTERNAL_ENGINE&gt;.&lt;OPTION_NAME&gt;. Option value syntax is
   *          dependent on the option.
   */
  const std::map<std::string, std::string>& GetOptions() const;

  /**
   * @brief Get full map of additional session settings
   * @return ref map of additional options
   */
  std::map<std::string, std::string>& GetOptions();

  /**
   * @brief Get all option names
   * @return vector of all additional option names
   */
  std::vector<std::string> GetOptionNames() const;

  /**
   * @brief Checks is there is a set additional option by name
   * @param name - string representation of option name
   * @return true if there is a set option with provided name
   */
  bool HasOption(const std::string& name) const;

  /**
   * @brief Get an additional option value by name
   * @param name - string representation of option name
   * @return string value of an option
   *
   * @throws std::invalid_argument if there is no such option
   */
  const std::string& GetOption(
      const std::string& name) const throw(std::exception);

  /**
   * @brief Set(modify) an additional option value by name
   * @param name - string representation of option name
   * @param value - value of option to set
   */
  void SetOption(const std::string& name, const std::string& value);

  /**
   * @brief Remove an option from session settings (by name)
   * @param name - string representation of option name
   *
   * @throws std::invalid_argument if there is no such option
   */
  void RemoveOption(const std::string& name) throw(std::exception);

  /**
   * @brief Get list of enabled fields for document type
   */
  const std::map<std::string, std::vector<std::string> >&
      GetEnabledFieldNames() const;

  /**
   * @brief Enable fields by name
   * @param doctype - type of document
   * @param field_name - name of field
   */
  void EnableField(const std::string& doctype, const std::string& field_name);

  /**
   * @brief Disable string fields by name
   * @param doctype - type of document
   * @param field_name - name of field
   */
  void DisableField(const std::string& doctype, const std::string& field_name);

  /**
   * @brief Set(modify) an enabled string fields by names
   * @param doctype - type of document
   * @param field_names - list of string field names
   */
  void SetEnabledFields(
      const std::string& doctype, const std::vector<std::string>& field_names);

  /**
   * @brief Get set of enabled string fields
   * @param doctype - type of document
   * @return list of supported field names for document
   *
   * @throws std::invalid_argument if there is no such document
   */
  const std::vector<std::string>& GetSupportedFieldNames(
      const std::string& doctype) throw(std::exception);

  /**
   * @brief Get list of enabled document forensics field for given document type
   */
  const std::map<std::string, std::vector<std::string> >&
      GetEnabledForensicFieldNames() const;

  /**
   * @brief Enable document forensic fields by name
   * @param doctype - type of document
   * @param field_name - name of field
   */
  void EnableForensicField(const std::string& doctype,
                           const std::string& field_name);

  /**
   * @brief Disable document forensic fields by name
   * @param doctype - type of document
   * @param field_name - name of field
   */
  void DisableForensicField(const std::string& doctype,
                            const std::string& field_name);

  /**
   * @brief Set(modify) an enabled document forensic fields by names
   * @param doctype - type of document
   * @param field_names - list of string field names
   */
  void SetEnabledForensicFields(
      const std::string& doctype, const std::vector<std::string>& field_names);

  /**
   * @brief Get set of enabled document forensic fields
   * @param doctype - type of document
   * @return list of supported field names for document
   *
   * @throws std::invalid_argument if there is no such document
   */
  const std::vector<std::string>& GetSupportedForensicFieldNames(
      const std::string& doctype) throw(std::exception);

  /**
   * @brief Returns current bundle mode
   * @return string name of current bundle mode
   */
  const std::string& GetCurrentMode() const;

  /**
   * @brief Sets current bundle mode
   * @param mode - string name of new current bundle mode
   */
  void SetCurrentMode(const std::string& mode) throw(std::exception);

  /**
   * @brief Gets list of available bundle mode names
   * @return list of available modes
   */
  const std::vector<std::string>& GetAvailableModes() const;

protected:
  std::vector<std::string> supported_modes_;
  std::string current_mode_;

  std::map<std::string, std::vector<std::vector<std::string> > >
      supported_document_types_;
  std::map<std::string, std::vector<std::string> > enabled_document_types_;

  std::map<std::string, std::string> options_;
  std::map<std::string, std::map<std::string, std::vector<std::string> > >
      supported_fields_;
  std::map<std::string, std::map<std::string, std::vector<std::string> > >
      enabled_fields_;

  std::map<std::string, std::map<std::string, std::vector<std::string> > >
      supported_forensic_fields_;
  std::map<std::string, std::map<std::string, std::vector<std::string> > >
      enabled_forensic_fields_;

  /// Disabled default constructor - use RecognitionEngine factory method instead
  SessionSettings();
};

/**
 * @brief RecognitionSession class -
 *        main interface for SmartID document recognition in videostream
 */
class SMARTID_DLL_EXPORT RecognitionSession {
public:
  /// RecognitionSession dtor
  virtual ~RecognitionSession();

  /**
   * @brief Processes the uncompressed RGB image stored in memory line by line
   * @param data                Pointer to the data buffer beginning
   * @param data_length         Length of the data buffer
   * @param width               Image width
   * @param height              Image height
   * @param stride              Difference between the pointers to the
   *                            consequent image lines, in bytes
   * @param channels            Number of channels (1, 3 or 4). 1-channel image
   *                            is treated as grayscale image, 3-channel image
   *                            is treated as RGB image, 4-channel image is
   *                            treated as BGRA.
   * @param roi                 Rectangle of interest (the system will not
   *                            process anything outside this rectangle)
   * @param image_orientation   Current image orientation to perform proper
   *                            rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If processing error occurs
   */
  virtual RecognitionResult ProcessSnapshot(
      unsigned char* data,
      size_t data_length,
      int width,
      int height,
      int stride,
      int channels,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception) = 0;

  /**
   * @brief Processes the uncompressed RGB image stored in memory line by line.
   *        Same as ProcessSnapshot with ROI, but with this method the ROI is
   *        full image.
   * @param data                Pointer to the data buffer beginning
   * @param data_length         Length of the data buffer
   * @param width               Image width
   * @param height              Image height
   * @param stride              Difference between the pointers to the
   *                            consequent image lines, in bytes
   * @param channels            Number of channels (1, 3 or 4). 1-channel image
   *                            is treated as grayscale image, 3-channel image
   *                            is treated as RGB image, 4-channel image is
   *                            treated as BGRA.
   * @param image_orientation   Current image orientation to perform proper
   *                            rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If processing error occurs
   */
  virtual RecognitionResult ProcessSnapshot(
      unsigned char* data,
      size_t data_length,
      int width,
      int height,
      int stride,
      int channels,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Processes the uncompressed YUV image stored in memory line by line
   * @param  yuv_data             Pointer to the data buffer start
   * @param  yuv_data_length      Total length of image data buffer
   * @param  width                Image width
   * @param  height               Image height
   * @param  roi                  Rectangle of interest (the system will not
   *                              process anything outside this rectangle)
   * @param  image_orientation    Current image orientation to perform proper
   *                              rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If processing error occurs
   */
  virtual RecognitionResult ProcessYUVSnapshot(
      unsigned char* yuv_data,
      size_t yuv_data_length,
      int width,
      int height,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Processes the uncompressed YUV image stored in memory line by line.
   *         Same as ProcessYUVSnapshot with ROI, but with this method the ROI
   *         is full image
   * @param  yuv_data             Pointer to the data buffer start
   * @param  yuv_data_length      Total length of image data buffer
   * @param  width                Image width
   * @param  height               Image height
   * @param  image_orientation    Current image orientation to perform proper
   *                              rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If processing error occurs
   */
  virtual RecognitionResult ProcessYUVSnapshot(
      unsigned char* yuv_data,
      size_t yuv_data_length,
      int width,
      int height,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified smartid::Image
   * @param  image               An Image to process
   * @param  roi                 Rectangle of interest (the system will not
   *                             process anything outside this rectangle)
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If file doesn't exist or can't be processed, or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImage(
      const Image& image,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified smartid::Image.
   *         Same as ProcessImage with ROI, but with this method the ROI is
   *         full image
   * @param  image               An Image to process
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If file doesn't exist or can't be processed, or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImage(
      const Image& image,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified file
   * @param  image_file          Image file path
   * @param  roi                 Rectangle of interest (the system will not
   *                             process anything outside this rectangle)
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If file doesn't exist or can't be processed, or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageFile(
      const std::string& image_file,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified file.
   *         Same as ProcessImageFile with ROI, but with this method the ROI is
   *         full image
   * @param  image_file          Image file path
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If file doesn't exist or can't be processed, or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageFile(
      const std::string& image_file,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified image data (e.g.
   *         compressed with JPEG or PNG)
   * @param  data                Pointer to the (e.g. compressed) image data
   * @param  data_length         Compressed image data length in bytes
   * @param  roi                 Rectangle of interest (the system will not
   *                             process anything outside this rectangle)
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If image data can't be decoded or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageData(
      unsigned char* data,
      size_t data_length,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified image data (e.g.
   *         compressed with JPEG or PNG), using ROI as full image
   * @param  data                Pointer to the (e.g. compressed) image data
   * @param  data_length         Compressed image data length in bytes
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If image data can't be decoded or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageData(
      unsigned char* data,
      size_t data_length,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified image data (e.g.
   *         compressed with JPEG or PNG) encoded in base64
   * @param  base64_image_data   Encoded image
   * @param  roi                 Rectangle of interest (the system will not
   *                             process anything outside this rectangle)
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If image data can't be decoded or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageDataBase64(
      const std::string& base64_image_data,
      const Rectangle& roi,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Runs recognition process on the specified image data (e.g.
   *         compressed with JPEG or PNG) encoded in base64
   * @param  base64_image_data   Encoded image
   * @param  image_orientation   Current image orientation to perform proper
   *                             rotation to landscape
   *
   * @return recognition result (integrated in the session)
   * @throws std::exception     If image data can't be decoded or
   *                            if processing error occurs
   */
  virtual RecognitionResult ProcessImageDataBase64(
      const std::string& base64_image_data,
      ImageOrientation image_orientation = Landscape) throw(std::exception);

  /**
   * @brief  Gets session state object - optional information about OCR state
   * @return SessionState object. Caller is responsible for deallocation.
   * @throws std::exception if the session state cannot be created
   */
  virtual SessionState* GetSessionState() const throw(std::exception) = 0;

  /**
   * @brief Resets the internal state of the session
   */
  virtual void Reset() = 0;
};

/**
 * @brief The RecognitionEngine class - a factory for RecognitionSessions,
 *        holds configured internal engines.
 */
class SMARTID_DLL_EXPORT RecognitionEngine {
public:
  /**
   * @brief RecognitionEngine ctor from configuration path
   * @param config_path - path to configuration file
   * @param lazy_configuration - whether to use engine's
   *    lazy component configuration capabilities
   *
   * @throws std::exception if configuration error occurs
   */
  RecognitionEngine(const std::string& config_path,
                    bool lazy_configuration = true) throw(std::exception);

  /**
   * @brief RecognitionEngine ctor from configuration buffer. Only for
   *        configuration from ZIP archive buffers.
   * @param config_data - pointer to configuration ZIP buffer start
   * @param data_length - size of the configuration ZIP buffer
   * @param lazy_configuration - whether to use engine's
   *    lazy component configuration capabilities
   *
   * @throws std::exception if configuration error occurs
   */
  RecognitionEngine(unsigned char* config_data,
                    size_t data_length,
                    bool lazy_configuration = true) throw(std::exception);

  /// Recognition Engine dtor
  ~RecognitionEngine();

  /**
   * @brief Factory method for creating 'default' session settings
   *        with options loaded from configured bundle and no enabled documents
   * @return Allocated session settings, caller is responsible for destruction
   * @throws std::exception if settings creation failed
   */
  SessionSettings* CreateSessionSettings() const throw(std::exception);

  /// Sessions for videostream recognition (one document - multiple frames)

  /**
   * @brief Factory method for creating a session for SmartId internal engine
   * @param session_settings - runtime session settings
   * @param result_reporter - pointer to optional processing reporter
   *        implementation
   *
   * @return pointer to created recognition session. The caller is responsible
   *         for session's destruction.
   * @throws std::exception if session creation failed
   */
  RecognitionSession* SpawnSession(
      const SessionSettings& session_settings,
      ResultReporterInterface* result_reporter = 0) const throw(std::exception);

  /**
   * @brief Gets RecognitionEngine library version
   * @return std::string version representation
   */
  static std::string GetVersion();

private:
  /// Disabled copy constructor
  RecognitionEngine(const RecognitionEngine& copy);
  /// Disabled assignment operator
  void operator=(const RecognitionEngine& other);

private:
  class RecognitionEngineImpl* pimpl_; ///< pointer to internal implementation
};
} } // namespace se::smartid

#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // SMARTID_ENGINE_SMARTID_ENGINE_H_INCLUDED
