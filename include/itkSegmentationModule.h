/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkSegmentationModule_h
#define itkSegmentationModule_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"
#include "itkSpatialObject.h"

namespace itk
{

/** \class SegmentationModule
 * \brief Class provides the abstract interface of the segmentation methods.
 *
 * The typical use of this class would be to generate a binary mask spatial
 * object representing a segmented object.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT SegmentationModule : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(SegmentationModule);

  /** Standard class type alias. */
  using Self = SegmentationModule;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SegmentationModule, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject< NDimension >;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void SetInput( const SpatialObjectType * input );
  const SpatialObjectType * GetInput() const;

  /** Input data that carries the feature in the form of a
   * SpatialObject. */
  void SetFeature( const SpatialObjectType * feature );
  const SpatialObjectType * GetFeature() const;

  /** Output segmentation represented as a SpatialObject. */
  const SpatialObjectType * GetOutput() const;

  /** Returns the number of features that are expected by this Segmentation
   * Module. This method will be overloaded in derived classes. */
  unsigned int GetExpectedNumberOfFeatures() const;

protected:
  SegmentationModule();
  ~SegmentationModule() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /* Derived classes MUST implement the "void  GenerateData ()" method */

  /** Output segmentation represented as a SpatialObject. Non-const version
   * only for internal use. */
  SpatialObjectType * GetInternalOutput();
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkSegmentationModule.hxx"
#endif

#endif
