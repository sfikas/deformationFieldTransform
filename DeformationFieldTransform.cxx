#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVector.h"
#if ITK_VERSION_MAJOR < 4
#include "itkDeformationFieldTransform.h"
#include "itkDeformationFieldSource.h"
#else
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkDisplacementFieldTransform.h"
#include "itkLandmarkDisplacementFieldSource.h"
#endif
#include "itkResampleImageFilter.h"

const     unsigned int   Dimension = 3;
typedef   float  PixelType;
typedef   itk::Image< PixelType, Dimension > ImageType;

template <unsigned int d> void readImage(const char *fn);


int main(int argc, char * argv[])
{
  //readImage<3>(argv[1]);
  ImageType::Pointer inputImage = ImageType::New();
  typedef itk::ImageFileReader<ImageType>            ImageReaderType;
  typename ImageReaderType::Pointer ImageReader = ImageReaderType::New();
  ImageReader->SetFileName(argv[1]);

  //
  // Read the input volume (argument 1)
  //
  try {
    ImageReader->Update();
    inputImage = ImageReader->GetOutput();
  }
  catch( itk::ExceptionObject& err ) {
    std::cout << "Could not read the input images." << std::endl;
    std::cout << err << std::endl;
    exit( EXIT_FAILURE );
  }

  ImageType::RegionType region = inputImage->GetLargestPossibleRegion();
  ImageType::SizeType size = region.GetSize();    
  std::cout << "Read input file with size ";
  std::cout << size << std::endl;

  //
  // Read the input deformation (argument 2)
  //
#if ITK_VERSION_MAJOR < 4
  typedef   float           VectorComponentType;
#else
  typedef   double          VectorComponentType;
#endif
  typedef itk::Vector< VectorComponentType, Dimension >    VectorType;  
  typedef itk::Image< VectorType,  Dimension >   DeformationFieldType;
  DeformationFieldType::Pointer inputDeformationField = DeformationFieldType::New();
  typedef itk::ImageFileReader<DeformationFieldType> DeformationFieldReaderType;
  typename DeformationFieldReaderType::Pointer DeformationFieldReader = DeformationFieldReaderType::New();
  DeformationFieldReader->SetFileName(argv[2]);

  //
  // Read the input volume (argument 1)
  //
  try {
    DeformationFieldReader->Update();
    inputDeformationField = DeformationFieldReader->GetOutput();
  }
  catch( itk::ExceptionObject& err ) {
    std::cout << "Could not read the input deformation field." << std::endl;
    std::cout << err << std::endl;
    exit( EXIT_FAILURE );
  }

  DeformationFieldType::RegionType df_region = inputDeformationField->GetLargestPossibleRegion();
  DeformationFieldType::SizeType df_size = df_region.GetSize();    
  std::cout << "Read input file with size ";
  std::cout << df_size << std::endl;

/*
#if ITK_VERSION_MAJOR < 4
  typedef itk::DeformationFieldSource<DeformationFieldType>  DeformationFieldSourceType;
#else
  typedef itk::LandmarkDisplacementFieldSource<DeformationFieldType>  DeformationFieldSourceType;
#endif
  DeformationFieldSourceType::Pointer deformationFieldSource = DeformationFieldSourceType::New();
  deformationFieldSource->SetOutputSpacing( inputImage->GetSpacing() );
  deformationFieldSource->SetOutputOrigin(  inputImage->GetOrigin() );
  deformationFieldSource->SetOutputRegion(  inputImage->GetLargestPossibleRegion() );
  deformationFieldSource->SetOutputDirection( inputImage->GetDirection() );

  // Write the deformation field
  {
  typedef itk::ImageFileWriter<  DeformationFieldType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  deformationFieldSource->GetOutput() );
  writer->SetFileName( "deformation.mhd" );
  writer->Update();
  }

  //  Create source and target landmarks.
  typedef DeformationFieldSourceType::LandmarkContainerPointer   LandmarkContainerPointer;
  typedef DeformationFieldSourceType::LandmarkContainer          LandmarkContainerType;
  typedef DeformationFieldSourceType::LandmarkPointType          LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks = LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks = LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;

  sourcePoint[0] = 40;
  sourcePoint[1] = 40;
  targetPoint[0] = 20;
  targetPoint[1] = 20;
  sourceLandmarks->InsertElement( 0, sourcePoint );
  targetLandmarks->InsertElement( 0, targetPoint );

  sourcePoint[0] = 40;
  sourcePoint[1] = 60;
  targetPoint[0] = 20;
  targetPoint[1] = 80;
  sourceLandmarks->InsertElement( 1, sourcePoint );
  targetLandmarks->InsertElement( 1, targetPoint );

  sourcePoint[0] = 60;
  sourcePoint[1] = 40;
  targetPoint[0] = 80;
  targetPoint[1] = 20;
  sourceLandmarks->InsertElement( 2, sourcePoint );
  targetLandmarks->InsertElement( 2, targetPoint );

  sourcePoint[0] = 60;
  sourcePoint[1] = 60;
  targetPoint[0] = 80;
  targetPoint[1] = 80;
  sourceLandmarks->InsertElement( 3, sourcePoint );
  targetLandmarks->InsertElement( 3, targetPoint );

  deformationFieldSource->SetSourceLandmarks( sourceLandmarks.GetPointer() );
  deformationFieldSource->SetTargetLandmarks( targetLandmarks.GetPointer() );
  deformationFieldSource->UpdateLargestPossibleRegion();

  // Write the deformation field
  {
  typedef itk::ImageFileWriter<  DeformationFieldType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  deformationFieldSource->GetOutput() );
  writer->SetFileName( "deformation.mhd" );
  writer->Update();
  }
*/

/*
#if ITK_VERSION_MAJOR < 4
  typedef itk::DeformationFieldTransform<VectorComponentType, Dimension>  DeformationFieldTransformType;
#else
  typedef itk::DisplacementFieldTransform<VectorComponentType, Dimension>  DeformationFieldTransformType;
#endif
  DeformationFieldTransformType::Pointer deformationFieldTransform = DeformationFieldTransformType::New();


#if ITK_VERSION_MAJOR < 4
  deformationFieldTransform->SetDeformationField( deformationFieldSource->GetOutput() );
#else
  deformationFieldTransform->SetDisplacementField( deformationFieldSource->GetOutput() );
#endif
*/
  typedef itk::ResampleImageFilter<ImageType, ImageType, VectorComponentType >    ResampleFilterType;
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetInput( inputImage );
  //resampleFilter->SetTransform( deformationFieldTransform );
  resampleFilter->SetSize( inputImage->GetLargestPossibleRegion().GetSize() );
  resampleFilter->SetOutputOrigin(  inputImage->GetOrigin() );
  resampleFilter->SetOutputSpacing( inputImage->GetSpacing() );
  resampleFilter->SetOutputDirection( inputImage->GetDirection() );
  resampleFilter->SetDefaultPixelValue( 200 );
  resampleFilter->GetOutput();


  // Write the output
  typedef itk::ImageFileWriter<  ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  resampleFilter->GetOutput() );
  writer->SetFileName( "output.nii" );
  writer->Update();
  
  return EXIT_SUCCESS;
}


template <unsigned int d> void readImage(const char *fn)
{
  // Verify command line arguments

  typedef unsigned char PixelType;

  typedef itk::Image< PixelType, d >        ImageType;
  typedef itk::ImageFileReader< ImageType > ReaderType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename ImageType::Pointer image;   
  typename ImageType::RegionType region;
  typename ImageType::SizeType size;

  reader->SetFileName(fn);
  reader->Update();

  image = reader->GetOutput();
  region = image->GetLargestPossibleRegion();
  size = region.GetSize();

  std::cout << size << std::endl;

  // An example image had w = 200 and h = 100
  // (it is wider than it is tall). The above output
  // 200 x 100
  // so w = GetSize()[0]
  // and h = GetSize()[1]

  // A pixel inside the region
  typename ImageType::IndexType indexInside;
  indexInside[0] = 150;
  indexInside[1] = 50;
  std::cout << region.IsInside(indexInside) << std::endl;

  // A pixel outside the region
  typename ImageType::IndexType indexOutside;
  indexOutside[0] = 50;
  indexOutside[1] = 150;
  std::cout << region.IsInside(indexOutside) << std::endl;

  // This means that the [0] component of the index is referencing the
  // left to right (x) value and the [1] component of Index is referencing
  // the top to bottom (y) value

}