import sys
import os

sys.path.append(os.path.join(sys.path[0], '../../bindings/python/'))
sys.path.append(os.path.join(sys.path[0], '../../bin/'))

import pySmartIdEngine


def output_recognition_result(result):
    print('Document type: %s' % result.GetDocumentType())
    print('Match results:')
    match_results = result.GetMatchResults()
    for i in range(len(match_results)):
        match_result_quad = match_results[i].GetQuadrangle()
        print('    Template Type = %s' % match_results[i].GetTemplateType())
        print('    Zone = { (%f, %f), (%f, %f), (%f, %f), (%f, %f) }' % (
            match_result_quad.GetPoint(0).x, match_result_quad.GetPoint(0).y,
            match_result_quad.GetPoint(1).x, match_result_quad.GetPoint(1).y,
            match_result_quad.GetPoint(2).x, match_result_quad.GetPoint(2).y,
            match_result_quad.GetPoint(3).x, match_result_quad.GetPoint(3).y))

    print('Segmentation results:')
    segmentation_results = result.GetSegmentationResults()
    for i in range(len(segmentation_results)):
        raw_fields_names = segmentation_results[i].GetRawFieldsNames()
        for j in range(len(raw_fields_names)):
            print('    Raw field %s' % raw_fields_names[j])

    print('String fields:')
    string_field_names = result.GetStringFieldNames()
    for i in range(len(string_field_names)):
        field = result.GetStringField(string_field_names[i])
        is_accepted = ' [+] ' if field.IsAccepted() else ' [-] '
        print('    %-20s %s %s' % (field.GetName(), is_accepted, field.GetUtf8Value()))

    print('Image fields:')
    image_field_names = result.GetImageFieldNames()
    for i in range(len(image_field_names)):
        field = result.GetImageField(image_field_names[i])
        is_accepted = ' [+] ' if field.IsAccepted() else ' [-] '
        print('    %-20s %s W: %d H: %d' % (field.GetName(), is_accepted,
                                            field.GetValue().GetWidth(), field.GetValue().GetHeight()))

    print('Result terminal:         %s' % (' [+] ' if result.IsTerminal() else ' [-] '))


def main():
    if len(sys.argv) != 4:
        print('Usage: %s <path-to-image-file> <path-to-config-bundle> <document-type>' % sys.argv[0])
        print('Example: %s ../testdata/image.png ../data-zip/bundle_mrz.zip mrz.*' % sys.argv[0])
        sys.exit(-1)

    image_path, config_path, document_types = sys.argv[1:]

    print('RecognitionEngine Version: %s' % pySmartIdEngine.RecognitionEngine.GetVersion())
    print('Creating RecognitionEngine from %s' % config_path)

    engine = pySmartIdEngine.RecognitionEngine(config_path)
    
    session_settings = engine.CreateSessionSettings()
    set_enabled_document_types = pySmartIdEngine.StringVector()
    set_enabled_document_types.append(document_types)
    session_settings.SetEnabledDocumentTypes(set_enabled_document_types)

# load image 

    testImage = pySmartIdEngine.Image()

  # This works only in trial and full versions

    # testImage = pySmartIdEngine.Image(image_path) 

# get file as binary buffer 

    binaryRGBfile = open(image_path, "rb")
    size = os.path.getsize(image_path)
    binaryRGBfileBuffer = bytearray(size)
    binaryRGBfile.readinto(binaryRGBfileBuffer)

# get raw RGB data from file 

  # You can take image buffer directly from Image object, works in trial and full versions

    # bufferSizeForRGB = testImage.GetHeight()*testImage.GetStride()*testImage.GetChannels()
    # bufferForRGB = bytearray(bufferSizeForRGB)
    # testImage.CopyToBuffer(bufferForRGB, bufferSizeForRGB)

# get file as Base64 string

    f = open(os.path.join(sys.path[0], "base64.txt"), 'r')
    base64BufferString = f.readline()
    f.close()

  # You can take base64 buffer directly from Image object

    # base64BufferSize = testImage.GetRequiredBase64BufferLength()  
    # base64Buffer = bytearray(base64BufferSize)
    # testImage.CopyBase64ToBuffer(base64Buffer, base64BufferSize)
    # base64BufferString = base64Buffer.decode()
    # base64BufferString = str(base64BufferString) #explicit conversion need for Python 2

  # or you can take base64 string directly by
    
    # base64BufferString = testImage.GetBase64String()

# start recognition session

    session = engine.SpawnSession(session_settings)

    resultFromImage = session.ProcessImage(testImage)
    session.Reset()

    resultFromImageFile = session.ProcessImageFile(image_path)
    session.Reset()

    resultFromImageData = session.ProcessImageData(binaryRGBfileBuffer, size)
    session.Reset()

    resultFromBase64 = session.ProcessImageDataBase64(base64BufferString)
    session.Reset()

    # resultFromRawBuffer = session.ProcessSnapshot(bufferForRGB, bufferSizeForRGB, testImage.GetWidth(), testImage.GetHeight(), testImage.GetStride(), testImage.GetChannels()) 

# print result

    output_recognition_result(resultFromImage)
    output_recognition_result(resultFromImageFile)
    output_recognition_result(resultFromImageData)
    output_recognition_result(resultFromBase64)
    # output_recognition_result(resultFromRawBuffer)



if __name__ == '__main__':
    main()


