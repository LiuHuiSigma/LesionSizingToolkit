/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFrangiTubularnessFeatureGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkFrangiTubularnessFeatureGenerator_h
#define itkFrangiTubularnessFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricSecondRankTensor.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkFrangiTubularnessImageFilter.h"

namespace itk
{

/** \class FrangiTubularnessFeatureGenerator
 * \brief Generates a feature image by computing measures based on the Hessian Eigenvalues.
 *
 * The typical use of this class would be to generate a map of {blobs, tubes, sheets}.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT FrangiTubularnessFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = FrangiTubularnessFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FrangiTubularnessFeatureGenerator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image< InputPixelType, Dimension >;
  using InputImageSpatialObjectType = ImageSpatialObject< NDimension, InputPixelType >;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void SetInput( const SpatialObjectType * input );
  const SpatialObjectType * GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType * GetFeature() const;

  /** Sigma value to be used in the Gaussian smoothing preceding the
   * Hessian computation. */
  itkSetMacro( Sigma, double );
  itkGetMacro( Sigma, double );

  /** Sheetness normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro( SheetnessNormalization, double );
  itkGetMacro( SheetnessNormalization, double );

  /** Bloobiness normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro( BloobinessNormalization, double );
  itkGetMacro( BloobinessNormalization, double );

  /** Noise normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro( NoiseNormalization, double );
  itkGetMacro( NoiseNormalization, double );

protected:
  FrangiTubularnessFeatureGenerator();
  ~FrangiTubularnessFeatureGenerator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  FrangiTubularnessFeatureGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  using InternalPixelType = float;
  using InternalImageType = Image< InternalPixelType, Dimension >;

  using OutputPixelType = InternalPixelType;
  using OutputImageType = InternalImageType;

  using OutputImageSpatialObjectType = ImageSpatialObject< NDimension, OutputPixelType >;

  using HessianFilterType = HessianRecursiveGaussianImageFilter< InputImageType >;
  using HessianImageType = typename HessianFilterType::OutputImageType;
  using HessianPixelType = typename HessianImageType::PixelType;

  using EigenValueArrayType = FixedArray< double, HessianPixelType::Dimension >;
  using EigenValueImageType = Image< EigenValueArrayType, Dimension >;

  using EigenAnalysisFilterType = SymmetricEigenAnalysisImageFilter< HessianImageType, EigenValueImageType >;
 
  using SheetnessFilterType = FrangiTubularnessImageFilter< EigenValueImageType, OutputImageType >;

  typename HessianFilterType::Pointer             m_HessianFilter;
  typename EigenAnalysisFilterType::Pointer       m_EigenAnalysisFilter;
  typename SheetnessFilterType::Pointer           m_SheetnessFilter;

  double      m_Sigma;
  double      m_SheetnessNormalization;
  double      m_BloobinessNormalization;
  double      m_NoiseNormalization;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkFrangiTubularnessFeatureGenerator.hxx"
#endif

#endif
