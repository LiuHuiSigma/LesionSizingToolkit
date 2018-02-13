/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGradientMagnitudeSigmoidFeatureGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkGradientMagnitudeSigmoidFeatureGenerator_h
#define itkGradientMagnitudeSigmoidFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"

namespace itk
{

/** \class GradientMagnitudeSigmoidFeatureGenerator
 * \brief Generates a feature image by computing the gradient magnitude of the
 * input image and applying a sigmoid transformation to it.
 *
 * The typical use of this class would be to generate the edge-map needed by a
 * Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT GradientMagnitudeSigmoidFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = GradientMagnitudeSigmoidFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GradientMagnitudeSigmoidFeatureGenerator, FeatureGenerator);

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

  /** Sigma value to be used in the Gaussian smoothing preceeding the gradient
   * magnitude computation. */
  itkSetMacro( Sigma, double );
  itkGetMacro( Sigma, double );

  /** Alpha value to be used in the Sigmoid filter. */
  itkSetMacro( Alpha, double );
  itkGetMacro( Alpha, double );

  /** Beta value to be used in the Sigmoid filter. */
  itkSetMacro( Beta, double );
  itkGetMacro( Beta, double );

protected:
  GradientMagnitudeSigmoidFeatureGenerator();
  ~GradientMagnitudeSigmoidFeatureGenerator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  GradientMagnitudeSigmoidFeatureGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  using InternalPixelType = float;
  using InternalImageType = Image< InternalPixelType, Dimension >;

  using OutputPixelType = InternalPixelType;
  using OutputImageType = InternalImageType;

  using OutputImageSpatialObjectType = ImageSpatialObject< NDimension, OutputPixelType >;

  using GradientFilterType = GradientMagnitudeRecursiveGaussianImageFilter<
    InputImageType, InternalImageType >;
  using GradientFilterPointer = typename GradientFilterType::Pointer;

  using SigmoidFilterType = SigmoidImageFilter<
    InternalImageType, OutputImageType >;
  using SigmoidFilterPointer = typename SigmoidFilterType::Pointer;

  GradientFilterPointer           m_GradientFilter;
  SigmoidFilterPointer            m_SigmoidFilter;

  double                          m_Sigma;
  double                          m_Alpha;
  double                          m_Beta;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkGradientMagnitudeSigmoidFeatureGenerator.hxx"
#endif

#endif
