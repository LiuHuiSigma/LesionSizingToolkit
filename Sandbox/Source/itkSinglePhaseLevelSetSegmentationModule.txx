/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSinglePhaseLevelSetSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSinglePhaseLevelSetSegmentationModule_txx
#define __itkSinglePhaseLevelSetSegmentationModule_txx

#include "itkSinglePhaseLevelSetSegmentationModule.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>
::SinglePhaseLevelSetSegmentationModule()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 1 );

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput( 0, outputObject.GetPointer() );

  this->m_MaximumNumberOfIterations = 100;
  this->m_MaximumRMSError = 0.01;

  this->m_AdvectionScaling = 1.0;
  this->m_CurvatureScaling = 75.0;
  this->m_PropagationScaling = 100.0;
}


/**
 * Destructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>
::~SinglePhaseLevelSetSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "PropagationScaling = " << this->m_PropagationScaling << std::endl;
  os << indent << "CurvatureScaling = " << this->m_CurvatureScaling << std::endl;
  os << indent << "AdvectionScaling = " << this->m_AdvectionScaling << std::endl;
  os << indent << "MaximumRMSError = " << this->m_MaximumRMSError << std::endl;
  os << indent << "MaximumNumberOfIterations = " << this->m_MaximumNumberOfIterations << std::endl;
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
