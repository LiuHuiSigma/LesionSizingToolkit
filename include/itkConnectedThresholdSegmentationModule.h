/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConnectedThresholdSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkConnectedThresholdSegmentationModule_h
#define itkConnectedThresholdSegmentationModule_h

#include "itkRegionGrowingSegmentationModule.h"
#include "itkConnectedThresholdImageFilter.h"

namespace itk
{

/** \class ConnectedThresholdSegmentationModule
 * \brief This class applies the connected threshold region growing
 * segmentation method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT ConnectedThresholdSegmentationModule : 
  public RegionGrowingSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(ConfidenceConnectedSegmentationModule);

  /** Standard class type alias. */
  using Self = ConnectedThresholdSegmentationModule;
  using Superclass = RegionGrowingSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConnectedThresholdSegmentationModule, RegionGrowingSegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using FeatureImageType = typename Superclass::FeatureImageType;
  using OutputImageType = typename Superclass::OutputImageType;
  using InputSpatialObjectType = typename Superclass::InputSpatialObjectType;

  /** Upper and Lower thresholds used to control the region growth. */
  itkSetMacro( LowerThreshold, double );
  itkGetMacro( LowerThreshold, double );
  itkSetMacro( UpperThreshold, double );
  itkGetMacro( UpperThreshold, double );

protected:
  ConnectedThresholdSegmentationModule();
  ~ConnectedThresholdSegmentationModule() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  double        m_LowerThreshold;
  double        m_UpperThreshold;
  
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkConnectedThresholdSegmentationModule.hxx"
#endif

#endif
