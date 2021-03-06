/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSatoVesselnessFeatureGeneratorMultiScaleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSatoVesselnessFeatureGenerator.h"
#include "itkMaximumFeatureAggregator.h"


int itkSatoVesselnessFeatureGeneratorMultiScaleTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage ";
    std::cerr << " [sigma alpha1 alpha2]" << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[1] );

  try
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  using AggregatorType = itk::MaximumFeatureAggregator< Dimension >;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();

  using FeatureGeneratorType = itk::SatoVesselnessFeatureGenerator< Dimension >;
  using SpatialObjectType = FeatureGeneratorType::SpatialObjectType;

  FeatureGeneratorType::Pointer  featureGenerator1 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator2 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator3 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator4 = FeatureGeneratorType::New();

  if( argc > 3 )
    {
    double smallestSigma = atof( argv[3] );
    featureGenerator1->SetSigma( smallestSigma );
    featureGenerator2->SetSigma( smallestSigma * 2.0 );
    featureGenerator3->SetSigma( smallestSigma * 4.0 );
    featureGenerator4->SetSigma( smallestSigma * 8.0 );
    }

  if( argc > 4 )
    {
    featureGenerator1->SetAlpha1( atof( argv[4] ) );
    featureGenerator2->SetAlpha1( atof( argv[4] ) );
    featureGenerator3->SetAlpha1( atof( argv[4] ) );
    featureGenerator4->SetAlpha1( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator1->SetAlpha2( atof( argv[5] ) );
    featureGenerator2->SetAlpha2( atof( argv[5] ) );
    featureGenerator3->SetAlpha2( atof( argv[5] ) );
    featureGenerator4->SetAlpha2( atof( argv[5] ) );
    }

  using SpatialObjectType = AggregatorType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator1->SetInput( inputObject );
  featureGenerator2->SetInput( inputObject );
  featureGenerator3->SetInput( inputObject );
  featureGenerator4->SetInput( inputObject );

  featureAggregator->AddFeatureGenerator( featureGenerator1 );
  featureAggregator->AddFeatureGenerator( featureGenerator2 );
  featureAggregator->AddFeatureGenerator( featureGenerator3 );
  featureAggregator->AddFeatureGenerator( featureGenerator4 );

  try
    {
    featureAggregator->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  SpatialObjectType::ConstPointer finalFeature = featureAggregator->GetFeature();

  using OutputImageSpatialObjectType = AggregatorType::OutputImageSpatialObjectType;
  using OutputImageType = AggregatorType::OutputImageType;

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast< const OutputImageSpatialObjectType * >( finalFeature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  featureAggregator->Print( std::cout );

  return EXIT_SUCCESS;
}
