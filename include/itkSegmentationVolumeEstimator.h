/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSegmentationVolumeEstimator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkSegmentationVolumeEstimator_h
#define itkSegmentationVolumeEstimator_h

#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkSimpleDataObjectDecorator.h"

namespace itk
{

/** \class SegmentationVolumeEstimator
 * \brief Class for estimating the volume of a segmentation stored in a SpatialObject
 *
 * This class is the base class for specific implementation of volume estimators.
 *
 * SpatialObjects are used as input of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT SegmentationVolumeEstimator : public ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = SegmentationVolumeEstimator;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** This is an abstract class, therefore it doesn't need the itkNewMacro() */

  /** Run-time type information (and related methods). */
  itkTypeMacro(SegmentationVolumeEstimator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject< NDimension >;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;
  using SpatialObjectConstPointer = typename SpatialObjectType::ConstPointer;

  /** Set the input SpatialObject representing the segmentation whose volume
   * will be estimated */
  using ProcessObject::SetInput;
  void SetInput( const SpatialObjectType * inputSpatialObject );

  /** Type of DataObjects used for scalar outputs */
  using RealType = double;
  using RealObjectType = SimpleDataObjectDecorator< RealType >;

  /** Return the computed Volume. The volume units will be relative to the
 * spacing units used by the input spatial object. For example, if the input
 * spatial object is using millimeters as the units of spacing then the units
 * of the volume computed in this class will be cubic millimeters. */
  RealType GetVolume() const;
  const RealObjectType * GetVolumeOutput() const;


protected:
  SegmentationVolumeEstimator();
  ~SegmentationVolumeEstimator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData() override;

private:
  SegmentationVolumeEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkSegmentationVolumeEstimator.hxx"
#endif

#endif
