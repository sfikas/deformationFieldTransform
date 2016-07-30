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
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkImageRegionConstIterator.h"

const     unsigned int   Dimension = 3;
typedef   float  PixelType;
typedef   itk::Image< PixelType, Dimension > ImageType;
// This is used to count voxels of images
typedef   itk::ImageRegionConstIterator< ImageType >  IteratorType;

std::size_t countNonzeroVoxels(ImageType::Pointer im, ImageType::RegionType region);

int main(int argc, char * argv[])
{
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

  std::cout << "Read " << countNonzeroVoxels(inputImage, region) << "voxels" << std::endl;  
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

  //
  // Apply the deformation
  //
#if ITK_VERSION_MAJOR < 4
  typedef itk::DeformationFieldTransform<VectorComponentType, Dimension>  DeformationFieldTransformType;
#else
  typedef itk::DisplacementFieldTransform<VectorComponentType, Dimension>  DeformationFieldTransformType;
#endif
  DeformationFieldTransformType::Pointer deformationFieldTransform = DeformationFieldTransformType::New();


#if ITK_VERSION_MAJOR < 4
  deformationFieldTransform->SetDeformationField( inputDeformationField );
#else
  deformationFieldTransform->SetDisplacementField( inputDeformationField );
#endif

  typedef itk::ResampleImageFilter<ImageType, ImageType, VectorComponentType >    ResampleFilterType;
  typedef   itk::NearestNeighborInterpolateImageFunction<ImageType> InterpolatorType;  
  InterpolatorType::Pointer interpolator = InterpolatorType::New();  
  
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetInput( inputImage );
  resampleFilter->SetTransform( deformationFieldTransform );
  resampleFilter->SetInterpolator( interpolator );
  resampleFilter->SetSize( inputImage->GetLargestPossibleRegion().GetSize() );
  resampleFilter->SetOutputOrigin(  inputImage->GetOrigin() );
  resampleFilter->SetOutputSpacing( inputImage->GetSpacing() );
  resampleFilter->SetOutputDirection( inputImage->GetDirection() );
  resampleFilter->SetDefaultPixelValue( 0 );
  resampleFilter->GetOutput();


  // Write the output
  typedef itk::ImageFileWriter<  ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  resampleFilter->GetOutput() );
  writer->SetFileName(argv[3]);
  writer->Update();
  
  std::cout << "Wrote " << countNonzeroVoxels(resampleFilter->GetOutput(), resampleFilter->GetOutput()->GetLargestPossibleRegion()) << "voxels" << std::endl;  
  return EXIT_SUCCESS;
}

std::size_t countNonzeroVoxels(ImageType::Pointer im, ImageType::RegionType region)
{
  IteratorType it( im, region );
  std::size_t counter = 0;
  
  it.GoToBegin();
  while (!it.IsAtEnd()) { 
    if(it.Value())
      counter++;
    ++it;
  }
  return counter;
}
